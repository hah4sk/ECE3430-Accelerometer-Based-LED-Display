# ECE3430-Accelerometer-Based-LED-Display
Project for ECE3430 - Introduction to Embedded SystemsUses Accelerometer and MSP430 to alter LED Display 

This is an embedded-level project that uses the MSP430 Launchpad in order to program the LED-based functionality. Attached to the launchpad is an accelerometer header board, which reads information regarding the orientation of the board in relation to 3D space.  Using an ADC and SPI protocol, the header board sends this information in terms of digital x,y, and z coordinates to the launchpad.  Based on these values, the launchpad calculates the board's current orientation and, using Pulse Width Modulation, turns on the LEDs on the accelerometer accordingly.  

Per the current design, a greater tilt towards a given LED produces a higher brightness for that LED.
