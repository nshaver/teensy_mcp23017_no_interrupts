# teensy_mcp23017_no_interrupts
Teensy 4.1 + multiple CQRobot i2c MCP23017, examples for input from buttons and output to LED, without interrupts.

Uses the Adafruit MCP23017 library, but not the latest version (2.x). I used version 1.2. The 2.x version would not compile for Teensy 4.1.

Polls the buttons on one MCP23017 every 5ms. Seems to be very responsive and Teensy still runs very fast, usually around 4.5 million loops/second. I was able to run at around 8M loops/second when using interrupts, but unless I need the extra speed I'm going to stick with polling.
