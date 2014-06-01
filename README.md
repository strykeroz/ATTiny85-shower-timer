ATtiny85 4 minute shower timer
==============================

ATtiny85 4 minute countdown timer with 4 LEDs and piezo buzzer, using Arduino core 

This project as discussed in this Arduino forum thread: http://forum.arduino.cc/index.php?topic=168450.0

Not only a practical timer, this was also to test the idea of using the reset pin on the microcontroller to step through a simple state machine, with the state information saved to the onboard EEPROM.  There is no power switch, rather the uC is put in a low power state after a time of inactivity.  The reset button is connected to the only switch.

Operation
---------
Pressing the switch wakes an inactive timer and puts it in the ready/waiting state with all 4 LEDs lit.
Pressing the switch again starts the countdown timer.  The relevant LED for how many minutes are left flashes.  Each time a minute is reached, that LED is extinguished and the piezo beeps the number of minutes remaining.
At the end of 4 minutes there is a sequence of beeps followed by a long beep then all LEDs are extinguished.

If the timer is left in the ready/waiting state for too long it returns to the low power state.

ATtiny85 pin assignments 
------------------------
(legs numbered 1..8 counterclockwise from leg marked with dot)

1. Reset, pull-up resistor to 5V and weatherproof switch to GND
2. D3 LED #4 (10mm red)
3. D4 piezo buzzer
4. GND
5. D0 LED #1 (10mm green)
6. D1 LED #2 (10mm green)
7. D2 LED #3 (10mm green)
8. +5V

Code
----
Written using the Arduino IDE and Arduino-Tiny cores , the target to select is the ATtiny85 @ 8MHz (internal oscillator, BOD disabled).  Look up how to program the ATtiny using Arduino, and programming it using Arduino as an IDE.  

The EEPROM is written using a circular queue for wear-levelling. Given the situation that's not probably necessary, but other state machines might have far more transitions to process so it's there fore completeness.

Tested installation
-------------------
To avoid requiring a voltage regulator the test version uses 4x rechargeable AAA cells in series to provide 4.8V supply voltage max.  This has proven to give several months of use in a bathroom shared by 4 boys between charges.

BoM as tested:

- 1x ATtiny85-20PU
- 1x 8 pin DIP IC Socket (optional)
- 3x 10mm green diffused LED
- 1x 10mm red diffused LED
- 5x 330 Ohm resistors (current limiting, for LEDs & MOSFET)
- 1x 1k Ohm resistor (reset pull-up)
- 4x 10mm plastic LED holders, aluminium enclosure
- 1x 0.1uF ceramic capacitor (decoupling)
- 1x piezo buzzer
- 1x 2N7000 MOSFET drives piezo
- 1x 1M resistor (pulldown for MOSFET)
- 2x dual AAA battery pack
- 1x weatherproof IP65 momentary push button switch

Be sure to choose current limiting resistors to suit the particular LEDs you select.
