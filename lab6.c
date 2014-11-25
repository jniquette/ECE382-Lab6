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


	reqFunctionality();



	while(1)  {

		if (newIrPacket) {	//Got a new packet!
			_disable_interrupt();
			newIrPacket = FALSE;
			packetIndex = 0;

			if(IS_ONE){//if(packetBits == ONE){
				rightForward();
				leftForward();
			}
			else if(IS_TWO){//else if(packetBits == TWO){
				rightBackward();
				leftBackward();
			}

			_enable_interrupt();

		} // end if new IR packet arrived
	} // end infinite loop
} // end main

void stop(){
	DISABLE_RIGHT;
	DISABLE_LEFT;
}

void stepForward(){
	goForward();
	__delay_cycles(2000000);
	stop();
}

void stepBackward(){
	goBackward();
	__delay_cycles(2000000);
	stop();
}

void stepRight(){
	turnRight();
	__delay_cycles(2000000);
	stop();
}

void stepLeft(){
	turnLeft();
	__delay_cycles(2000000);
	stop();
}

void goForward(){
	rightForward();
	leftForward();
}

void goBackward(){
	rightBackward();
	leftBackward();
}

void turnRight(){
	rightBackward();
	leftForward();
}

void turnLeft(){
	leftBackward();
	rightForward();
}

void rightForward(){
	P1OUT |= BIT6;		//Green On
	RIGHT_FORWARD;
	ENABLE_RIGHT;
	TA1CCTL1 = OUTMOD_3;
}

void rightBackward(){
	P1OUT &= ~BIT6;		//Green Off
	RIGHT_BACKWARD;
	ENABLE_RIGHT;
	TA1CCTL1 = OUTMOD_7;
}

void leftForward(){
	P1OUT |= BIT0;		//Red On
	LEFT_FORWARD;
	ENABLE_LEFT;
	TA1CCTL2 = OUTMOD_7;
}

void leftBackward(){
	P1OUT &= ~BIT0;		//Red Off
	LEFT_BACKWARD;
	ENABLE_LEFT;
	TA1CCTL2 = OUTMOD_3;
}

void reqFunctionality(){
	stepForward();
	__delay_cycles(16000000);
	stepBackward();
	__delay_cycles(16000000);
	stepRight();
	__delay_cycles(16000000);
	stepLeft();
	__delay_cycles(16000000);
	stepForward();
	__delay_cycles(16000000);
	stepBackward();
}


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
//	P2DIR |= LEFT_MOTOR_POS;
//	P2DIR |= LEFT_MOTOR_NEG;
//	P2DIR |= RIGHT_MOTOR_POS;
//	P2DIR |= RIGHT_MOTOR_NEG;

	//Enable Motor Outputs
	P2DIR |= BIT0 | BIT1;			//Right Motor Enable and Direction
	P2DIR |= BIT3 | BIT5;			//Right Motor Enable and Direction
	
	//Enable the LEDs and turn them on (so I can see that the MSP is working)
	P1DIR |= BIT0 | BIT6;				// Enable updates to the LED
	P1OUT |= (BIT0 | BIT6);			// And turn the LEDa on

	//Setup Timer A0
	TA0CCR0 = 0x8000;					// create a 16mS roll-over period
	TA0CTL &= ~TAIFG;					// clear flag before enabling interrupts = good practice
	TA0CTL = ID_3 | TASSEL_2 | MC_1;		// Use 1:1 presclar off MCLK and enable interrupts

	//Setup Timer A1 and PWM
    P2DIR |= BIT2;							// P2.2 is associated with TA1CCR1
    P2SEL |= BIT2;							// P2.2 is associated with TA1CCTL1
    P2DIR |= BIT4;							// P2.4 is associated with TA1CCR2
    P2SEL |= BIT4;							// P2.4 is associated with TA1CCTL2
	TA1CTL = ID_3 | TASSEL_2 | MC_1;		// Use 1:8 presclar off MCLK
    TA1CCR0 = 0x0100;						// set signal period
    TA1CCR1 = 0x0008;//20;
    TA1CCTL1 = OUTMOD_7;				// set TACCTL1 to Reset / Set mode
    TA1CCR2 = 0x0008;//20;
    TA1CCTL2 = OUTMOD_7;					// set TACCTL1 to Reset / Set mode

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
		TA0CTL &= ~MC_0;

		LOW_2_HIGH; 				// Setup pin interrupr on positive edge
		break;

	case 1:							// !!!!!!!!POSITIVE EDGE!!!!!!!!!!!
		//Turn on Timer A
		TA0CTL |= MC_1;

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
	TA0CTL &= ~MC_0;

	//Disable Interrupt for timer A
	_disable_interrupt();

	//Clear Packet Index
	packetIndex = 0;

	//Set new Packet to true
	newIrPacket = TRUE;

	//Clear TAIFG
	TA0CTL &= ~TAIFG;
}
