ECE382-Lab6 Robot Motors and PWM
================================
The ECE382 Lab Instructions are [here](http://ece382.com/labs/lab6/index.html).

##Prelab
The prelab was graded in class with a score of 11/15.

###Hardware Used
- 1x MSP430G2553 on a TI MSP430G2 Launchpad
- 1x [SN754410 Motor Driver Datasheet](http://ece382.com/datasheets/SN754410.pdf)
- 1x [LDV33 3.3V Regulator Datasheet](http://ece382.com/datasheets/LD1117V33.pdf)
- 4x 0.1uF disc capacitor, two per motor
- 2x 10uF elec. capacitor, one for each voltage rail

###Wiring

There are two placement options for the MSP430 for this lab. The first is to leave the MSP430 on the launchpad and then wire the pins of the MSP430 to the breadboard. The second option is to remove the MSP430 from the launchpad and place it on the breadboard, running wires for VCC, TEST, and RESET between the Launchpad and the MSP430. Because I knew that I was going to want to use the two LEDs built into the Launchpad, and because I think it looks cleaner to do so, I opted to leave the MSP430 installed in the Launchpad.

###Hardware Schematic
Email the schematic to myself.

##Debugging

The first issue that I encountered was that I was unable to program my MSP430 while it the chip was on the breadboard and connected to the TI Launchpad by only the test, reset, ground, and VCC pins. The problem was a faulty VCC connection to the MSP430 due to one of the pins being bent upwards instead of being seated in the breadboard. Kudos to C2C Bapty on catching that one.

The second issue that I had was an unexpected rapid overheating of the SN754410 Quadruple Half-H Driver. I became aware of the problem when I could feel heat on my face while leaning over the board. The heat was so extreme that the driver actually melted the breadboard. My solution was to test another driver, which also overheated. Feeling discouraged, I checked the wiring of nearly every other robot in the lab only to find that they were all wired the same as mine and none of the others overheated. After approximately two hours of confusion, complete tear downs and rewiring, I decided to test a third chip. This third chip worked fine. I'll be looking for a hammer for the first two drivers (reference the movie "Office Space").

I did have the right motor working with forwards and backwards support. When I attempted to get the left motor wired up, I blew a fuse. I backed down to what I had before, and the motor would run for about three seconds before blowing another fuse. I attempted to reproduce exactly what I had to get just the right motor working, and the fuses continued to blow. After ~5 hours in the lab tonight I've given up for the evening.

On Monday night I was having some issues getting the left motor to work properly. After running through my code I saw that I had pins 2.0 and 2.1 (right motor enable and direction, respectively) set as output, but I had not yet configured pins 2.3 and 2.5 (left motor enable and direction, respectively). This was causing my left motor to work only intermittently. After correcting this I achieved required functionality. This was probably related to the earlier issue of getting an output of ~1.9V on these two pins.

##Required Functionality and A Functionality
Both functionalities are demonstrated in this [Youtube Video](https://www.youtube.com/watch?v=hhUQ_j3tiYA&list=UUAXhf3d2OrbDpP-IZgnnAvQ)

##Documentation
C2C El-Saawy and Dr. Coulston helped me deduce my wiring issues. I used C2C Yarbrough's Readme as a guideline for what types of content I should include.
