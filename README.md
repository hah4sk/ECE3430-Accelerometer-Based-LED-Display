# ECE3430-Accelerometer-Based-LED-Display
## Project for ECE3430 - Introduction to Embedded Systems - Using Accelerometer and MSP430 to alter LED Display 

This is an embedded-level project that uses the MSP430 Launchpad in order to program the LED-based functionality. Attached to the launchpad is an accelerometer header board, which reads information regarding the orientation of the board in relation to 3D space.  Using an ADC and SPI protocol, the header board sends this information in terms of digital x,y, and z coordinates to the launchpad.  Based on these values, the launchpad calculates the board's current orientation and, using Pulse Width Modulation, turns on the LEDs on the accelerometer accordingly.  

Per the current design, a greater tilt towards a given LED produces a higher brightness for that LED.


### Accelerometer
The MPS430 Launchpad uses an accelerometer headerboard to obtain information regarding the angle the launchpad is tilted.  The Accelerometer.c and Accelerometer.h files contain the code for calibrating the accelerometer, defining the max and min x,y, and z values available for the entity.

### Cordic
Cordic (COordinate Rotation DIgital Computer) is an algorithm used to efficiently calculate trig identities.  The Cordic.c and Cordic.h files are used to take the values obtained from the accelerometer, and convert them to angle measurements to determine the tilt of the board.

### Debounce
During the calibration process, the user must press a button available on the launchpad to step through the various calibration steps.  However, because the button is mechanical, it is susceptible to "debounce" issues, in which the button may bounce upon being pressed by the user and is effectively pressed multiple times on accident.  Debounce.c and Debounce.h define the state machine used to detect whether a button press is from the user or from the button's tendency to erroneously bounce multiple times. 

### Timer
Timer.c and Timer.h define the use a basic timer.  This is used by the debounce files in order to determine whether or not the user pressed the button upon a button-press.  If a certain period of time has passed without the button's "press status" changing, the press is determined to be done by the user and not by an erroneous debounce. 

### Pushbutton
The Pushbutton.c and Pushbutton.h files define the port on the board used for the calibration button. And allow interaction between the user and the board.

### SPI
SPI (Serial Peripheral Interface) is a serial communication specification that uses a "Master-Slave" protocol to communicate between two entities.  Spi.c and Spi.h define the communication used between the launchpad (master) and the accelerometer board (slave).

### ADC
ADC.c and ADC.h define the analog-to-digital conversion used by the accelerometer to convert its analog coordinate outputs to digital values that can be interpreted by the launchpad.

### LED
LED.c and LED.h define the ports used for the LED Display, and are used as visual indicators for the board's tilt.


