/*
 * lab6.h
 *
 *  Created on: Nov 19, 2014
 *      Author: C16Justin.Niquette
 */

#ifndef LAB6_H_
#define LAB6_H_

//----Method Declarations---
void stop();
void stepForward();
void stepBackward();
void stepRight();
void stepLeft();
void goForward();
void goBackward();
void turnRight();
void turnLeft();
void rightForward();
void rightBackward();
void leftForward();
void leftBackward();
void reqFunctionality();

//-----------------------------------------------------------------
// Page 76 : MSP430 Optimizing C/C++ Compiler v 4.3 User's Guide
//-----------------------------------------------------------------
typedef		unsigned char		int8;
typedef		unsigned short		int16;
typedef		unsigned long		int32;
typedef		unsigned long long	int64;

#define		TRUE				1
#define		FALSE				0

//-----------------------------------------------------------------
// Function prototypes found in lab5.c
//-----------------------------------------------------------------
void initMSP430();
__interrupt void pinChange (void);
__interrupt void timerOverflow (void);


//-----------------------------------------------------------------
// Define Macros Here for Self Commenting Code
// This setup uses software to reverse the direction of the 2nd motor
//-----------------------------------------------------------------
#define		RIGHT_DIRECTION		BIT1;
#define		RIGHT_ENABLE		BIT0;

#define		LEFT_DIRECTION		BIT5;
#define		LEFT_ENABLE			BIT3;
#define		LEFT_PWM			BIT4;

//#define		LEFT_FORWARD	P2OUT |= BIT0;
//#define		LEFT_BACKWARD	P2OUT &= ~BIT0;
#define		ENABLE_RIGHT	P2OUT |= RIGHT_ENABLE;
#define		DISABLE_RIGHT	P2OUT &= ~RIGHT_ENABLE;
#define		RIGHT_FORWARD	P2OUT &= ~RIGHT_DIRECTION;
#define		RIGHT_BACKWARD	P2OUT |= RIGHT_DIRECTION;

#define		ENABLE_LEFT		P2OUT |= LEFT_ENABLE;
#define		DISABLE_LEFT	P2OUT &= ~LEFT_ENABLE;
#define		LEFT_FORWARD	P2OUT |= LEFT_DIRECTION;
#define		LEFT_BACKWARD	P2OUT &= ~LEFT_DIRECTION;

//-----------------------------------------------------------------
// Each PxIES bit selects the interrupt edge for the corresponding I/O pin.
//	Bit = 0: The PxIFGx flag is set with a low-to-high transition
//	Bit = 1: The PxIFGx flag is set with a high-to-low transition
//-----------------------------------------------------------------

#define		IR_PIN			(P2IN & BIT6)
#define		HIGH_2_LOW		P2IES |= BIT6
#define		LOW_2_HIGH		P2IES &= ~BIT6


#define		averageLogic0Pulse	540		//0x0200
#define		averageLogic1Pulse	540*3	//0x0645
#define		averageStartPulse	4433	//hi only	//low and hi = 13336	//0x1100
#define		minLogic0Pulse		375		//from confidence interval //averageLogic0Pulse - 100
#define		maxLogic0Pulse		705		//from confidence interval	//averageLogic0Pulse + 100
#define		minLogic1Pulse		375*3	//averageLogic1Pulse - 100
#define		maxLogic1Pulse		705*3	//averageLogic1Pulse + 100
#define		minStartPulse		averageStartPulse - 500
#define		maxStartPulse		averageStartPulse + 500

#define		POWER		0x2CD3750A
#define		ZERO		0x2CD3057A
#define		ONE			0x2CD32D52
#define		TWO			0x2CD33D42
#define		THREE		0x2CD33946

#define		BIG_UP		0x2CD36C13
#define		BIG_DOWN	0xACD3601F
#define		BIG_LEFT	0xACD3304F
#define		BIG_RIGHT	0xACD3205F

//Mask the 4xMSB of the remote code due to the AC vs 2C inconsistency
#define		IS_ONE		(0x0000FFFF & packetBits) == (0x0000FFFF & ONE)
#define		IS_TWO		(0x0000FFFF & packetBits) == (0x0000FFFF & TWO)

#endif /* LAB6_H_ */
