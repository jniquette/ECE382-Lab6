//-----------------------------------------------------------------
// Name:	Coulston
// File:	lab5.c
// Date:	Fall 2014
// Purp:	Demo the decoding of an IR packet
//-----------------------------------------------------------------
#include <msp430g2553.h>
#include "lab6.h"

int8	newIrPacket = FALSE;
int32	packetBits;
int8	packetIndex = 0;

// -----------------------------------------------------------------------
// -----------------------------------------------------------------------
void main(void) {

	initMSP430();				// Setup MSP to process IR and buttons

	while(1)  {

		if (newIrPacket) {	//Got a new packet!
			_disable_interrupt();
			newIrPacket = FALSE;
			packetIndex = 0;

			if(IS_ONE){//if(packetBits == ONE){
				P1OUT ^= BIT0;		//Alternate the Red LED
			}
			else if(IS_TWO){//else if(packetBits == TWO){
				P1OUT ^= BIT6;		//Alternate the Green LED
			}

			_enable_interrupt();

		} // end if new IR packet arrived
	} // end infinite loop
} // end main

// -----------------------------------------------------------------------
// In order to decode IR packets, the MSP430 needs to be configured to
// tell time and generate interrupts on positive going edges.  The
// edge sensitivity is used to detect the first incoming IR packet.
// The P2.6 pin change ISR will then toggle the edge sensitivity of
// the interrupt in order to measure the times of the high and low
// pulses arriving from the IR decoder.
//
// The timer must be enabled so that we can tell how long the pulses
// last.  In some degenerate cases, we will need to generate a interrupt
// when the timer rolls over.  This will indicate the end of a packet
// and will be used to alert main that we have a new packet.
// -----------------------------------------------------------------------
void initMSP430() {

	IFG1=0; 					// clear interrupt flag1
	WDTCTL=WDTPW+WDTHOLD; 		// stop WD

	BCSCTL1 = CALBC1_8MHZ;
	DCOCTL = CALDCO_8MHZ;

	P2SEL  &= ~BIT6;						// Setup P2.6 as GPIO not XIN
	P2SEL2 &= ~BIT6;
	P2DIR &= ~BIT6;
	P2IFG &= ~BIT6;						// Clear any interrupt flag
	P2IE  |= BIT6;						// Enable PORT 2 interrupt on pin change

	//Enable Outputs on Motor Output Pins
	P2DIR |= LEFT_MOTOR_POS;
	P2DIR |= LEFT_MOTOR_NEG;
	P2DIR |= RIGHT_MOTOR_POS;
	P2DIR |= RIGHT_MOTOR_NEG;
	
	//Enable the LEDs and turn them on (so I can see that the MSP is working)
	P1DIR |= BIT0 | BIT6;				// Enable updates to the LED
	P1OUT |= (BIT0 | BIT6);			// And turn the LEDa on

	TA0CCR0 = 0x8000;					// create a 16mS roll-over period
	TACTL &= ~TAIFG;					// clear flag before enabling interrupts = good practice
	TACTL = ID_3 | TASSEL_2 | MC_1;		// Use 1:1 presclar off MCLK and enable interrupts

	HIGH_2_LOW; 						// Setup pin interrupr on positive edge

	_enable_interrupt();
}



// -----------------------------------------------------------------------
// Since the IR decoder is connected to P2.6, we want an interrupt
// to occur every time that the pin changes - this will occur on
// a positive edge and a negative edge.
//
// Negative Edge:
// The negative edge is associated with end of the logic 1 half-bit and
// the start of the logic 0 half of the bit.  The timer contains the
// duration of the logic 1 pulse, so we'll pull that out, process it
// and store the bit in the global irPacket variable. Going forward there
// is really nothing interesting that happens in this period, because all
// the logic 0 half-bits have the same period.  So we will turn off
// the timer interrupts and wait for the next (positive) edge on P2.6
//
// Positive Edge:
// The positive edge is associated with the end of the logic 0 half-bit
// and the start of the logic 1 half-bit.  There is nothing to do in
// terms of the logic 0 half bit because it does not encode any useful
// information.  On the other hand, we going into the logic 1 half of the bit
// and the portion which determines the bit value, the start of the
// packet, or if the timer rolls over, the end of the ir packet.
// Since the duration of this half-bit determines the outcome
// we will turn on the timer and its associated interrupt.
// -----------------------------------------------------------------------
#pragma vector = PORT2_VECTOR			// This is from the MSP430G2553.h file

__interrupt void pinChange (void) {

	int8	pin;
	int16	pulseDuration;			// The timer is 16-bits

	if(packetIndex >= 31){
		newIrPacket = TRUE;
		P2IFG &= ~BIT6;			// Clear the interrupt flag to prevent immediate ISR re-entry
		return;
	}

	if (IR_PIN)		pin=1;	else pin=0;

	switch (pin) {					// read the current pin level
	case 0:						// !!!!!!!!!NEGATIVE EDGE!!!!!!!!!!
		pulseDuration = TAR;
		packetIndex++;
		if(minStartPulse < pulseDuration && pulseDuration < maxStartPulse){
			packetIndex = 0;
		}
		else if(minLogic0Pulse < pulseDuration && pulseDuration < maxLogic0Pulse){	//If the bit is 0
			packetBits = packetBits << 1;
			packetBits += 0;
		}
		else if(minLogic1Pulse < pulseDuration && pulseDuration < maxLogic1Pulse){	//If the bit is 1
			packetBits = packetBits << 1;
			packetBits += 1;
		}
		// Otherwise Ignore the bit

		//Turn off Timer A
		TACTL &= ~MC_0;

		LOW_2_HIGH; 				// Setup pin interrupr on positive edge
		break;

	case 1:							// !!!!!!!!POSITIVE EDGE!!!!!!!!!!!
		//Turn on Timer A
		TACTL |= MC_1;

		//Enable Interrupt for timer A
		P2IE |= ID_3;

		TAR = 0x0000;						// time measurements are based at time 0
		HIGH_2_LOW; 						// Setup pin interrupr on positive edge
		break;
	} // end switch

	P2IFG &= ~BIT6;			// Clear the interrupt flag to prevent immediate ISR re-entry

} // end pinChange ISR



// -----------------------------------------------------------------------
//			0 half-bit	1 half-bit		TIMER A COUNTS		TIMER A COUNTS
//	Logic 0	xxx
//	Logic 1
//	Start
//	End
//
// -----------------------------------------------------------------------
#pragma vector = TIMER0_A1_VECTOR			// This is from the MSP430G2553.h file
__interrupt void timerOverflow (void) {
	//Turn off Timer A
	TACTL &= ~MC_0;

	//Disable Interrupt for timer A
	_disable_interrupt();

	//Clear Packet Index
	packetIndex = 0;

	//Set new Packet to true
	newIrPacket = TRUE;

	//Clear TAIFG
	TACTL &= ~TAIFG;
}
