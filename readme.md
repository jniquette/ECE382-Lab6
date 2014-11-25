



Debugging

The first issue that I encountered was that I was unable to program my MSP430 while it the chip was on the breadboard and connected to the TI Launchpad by only the test, reset, ground, and VCC pins. The problem was a faulty VCC connection to the MSP430 due to one of the pins being bent upwards instead of being seated in the breadboard. Kudos to C2C Bapty on catching that one.

The second issue that I had was an unexpected rapid overheating of the SN754410 Quadruple Half-H Driver. I became aware of the problem when I could feel heat on my face while leaning over the board. The heat was so extreme that the driver actually melted the breadboard. My solution was to test another driver, which also overheated. Feeling discouraged, I checked the wiring of nearly every other robot in the lab only to find that they were all wired the same as mine and none of the others overheated. After approximately two hours of confusion, complete tear downs and rewiring, I decided to test a third chip. This third chip worked fine. I'll be looking for a hammer for the first two drivers (reference the movie "Office Space").

I did have the right motor working with forwards and backwards support. When I attempted to get the left motor wired up, I blew a fuse. I backed down to what I had before, and the motor would run for about three seconds before blowing another fuse. I attempted to reproduce exactly what I had to get just the right motor working, and the fuses continued to blow. After ~5 hours in the lab tonight I've given up for the evening.

On Monday night I was having some issues getting the left motor to work properly. After running through my code I saw that I had pins 2.0 and 2.1 (right motor enable and direction, respectively) set as output, but I had not yet configured pins 2.3 and 2.5 (left motor enable and direction, respectively). This was causing my left motor to work only intermittently. After correcting this I achieved required functionality. This was probably related to the earlier issue of getting an output of ~1.9V on these two pins.