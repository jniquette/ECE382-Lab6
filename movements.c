#include <msp430g2553.h>
#include "lab6.h"

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
	__delay_cycles(1600000);
	stop();
}

void stepLeft(){
	turnLeft();
	__delay_cycles(1600000);
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
