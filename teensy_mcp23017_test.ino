/*
	 tests use of multiple CQRobot mcp23017 boards with teensy 4.1
	 board #1 is i2c address 0x27, unsoldered, all pins used as inputs
	 board #2 is i2c address 0x26, soldered across A0, all pins used as outputs
	 board #2 is i2c address 0x25, soldered across A1, all pins used as outputs
 */
#include "Adafruit_MCP23017.h"

Adafruit_MCP23017 mcp1;
Adafruit_MCP23017 mcp2;
Adafruit_MCP23017 mcp3;

#define LED_PIN 13
#define MCP_INT_PIN 2

unsigned long loopcount=0;
unsigned long looptime_ms=0;
unsigned long thisloop_ms=0;
unsigned long nextblinkoff_ms=0;
unsigned long next_led_ms=0;
int next_led_no=0;
uint16_t ledState=0b1111111111111111;
uint16_t buttonState=0b1111111111111111;

unsigned long next_mcp_read=0;
unsigned long button_count=0;

void handleButtons(){
	buttonState=mcp1.readGPIOAB();
}

void setup() {  
	delay(500);
	Serial.println("setup started");

  mcp1.begin(7);
  mcp2.begin(6);
  mcp3.begin(5);

	pinMode(MCP_INT_PIN, INPUT_PULLUP);

	for (int i=0;i<16;i++){
  	mcp1.pinMode(i, INPUT); // set all pins on board #1 as inputs
  	mcp1.pullUp(i, HIGH);  // turn on a 100K pullup internally

  	mcp2.pinMode(i, OUTPUT); // set all pins on board #2 as outputs, for LEDs
  	mcp3.pinMode(i, OUTPUT); // set all pins on board #3 as outputs, for LEDs
	}

	mcp1.readGPIOAB();

  pinMode(LED_PIN, OUTPUT);
	digitalWrite(LED_PIN, LOW);

	Serial.println("setup finished");
}

void loop() {
	thisloop_ms=millis();

	// read MCP23017 every x ms
	if (thisloop_ms>=next_mcp_read){
		handleButtons();
		next_mcp_read=thisloop_ms+5;
	}

	loopcount++;
	if (thisloop_ms>=looptime_ms){
		Serial.println((String)"Loops/Sec: " + loopcount + "   Buttons: " + button_count);
		loopcount=0;
		looptime_ms=thisloop_ms+1000;
	}

	// turn off debugging blinks after a few ms
	if (thisloop_ms>nextblinkoff_ms){
		digitalWrite(LED_PIN, LOW);
	}

	if (buttonState!=0b1111111111111111){
		// when buttons are pressed, light the LEDs for those buttons
		if (ledState!=buttonState){
			for (int i=0;i<16;i++){
				if (i>7){
					//mcp3.digitalWrite(i-8, LOW);
				} else {
					bool thisBit=bitRead(buttonState, i);
					if (thisBit){
						mcp2.digitalWrite(i, LOW);
						mcp3.digitalWrite(i, LOW);
					} else {
						button_count++;
						mcp2.digitalWrite(i, HIGH);
						mcp3.digitalWrite(i, HIGH);
					}
				}
			}
			// keep up with which LEDs are lit
			ledState=buttonState;
		}
	} else {
		// when no buttons are pressed, cycle through LEDs
		if (thisloop_ms>=next_led_ms){
			for (int i=0;i<16;i++){
				bool thisBit=true;
				if (next_led_no==i){
					if (i>7){
						mcp3.digitalWrite(i-8, HIGH);
					} else {
						mcp2.digitalWrite(i, HIGH);
					}
					bool thisBit=false;
				} else {
					if (i>7){
						mcp3.digitalWrite(i-8, LOW);
					} else {
						mcp2.digitalWrite(i, LOW);
					}
				}
				// keep up with which LEDs are lit/not lit
				bitWrite(ledState, i, thisBit);
			}

			// cycle to next LED
			next_led_no++;
			if (next_led_no>15) next_led_no=0;
			next_led_ms=thisloop_ms+50;
		}
	}
}
