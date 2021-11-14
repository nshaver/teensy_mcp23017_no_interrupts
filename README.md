# teensy_mcp23017_no_interrupts
Teensy 4.1 + multiple CQRobot i2c MCP23017, examples for input from buttons and output to LED, without interrupts.

Uses the Adafruit MCP23017 library, but not the latest version (2.x). I used version 1.2. The 2.x version would not compile for Teensy 4.1.

This can be very slow because it is polling every loop. Using interrupts is much better if possible. See my other
project for an example with interrupts.
