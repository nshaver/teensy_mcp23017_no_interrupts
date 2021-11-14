/*
	 tests use of multiple CQRobot mcp23017 boards with teensy 4.1
	 board #1 is i2c address 0x27, unsoldered, all pins used as inputs
	 board #2 is i2c address 0x26, soldered across A0, all pins used as outputs
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Adafruit_MCP23017.h"

#define LED_PIN 13

Adafruit_MCP23017 mcp1;
Adafruit_MCP23017 mcp2;

unsigned long loopcount=0;
unsigned long looptime_ms=0;
unsigned long thisloop_ms=0;
unsigned long nextblinkoff_ms=0;

uint16_t buttonState=0b1111111111111111;

void handleButtons(uint16_t tmpInt){
	for (int i=0;i<16;i++){
		bool oldBit=bitRead(buttonState, i);
		bool newBit=bitRead(tmpInt, i);
		if (oldBit!=newBit){
			if (newBit==1){
				Serial.println((String)"button " + i + " released");
				mcp2.digitalWrite(i, LOW);
			} else {
				Serial.println((String)"button " + i + " pressed");
				mcp2.digitalWrite(i, HIGH);
			}
		}
	}
	buttonState=tmpInt;
}

void setup() {  
	delay(500);
	Serial.println("setup started");

  mcp1.begin(7);      // use default address 0
  mcp2.begin(6);      // use default address 0

	for (int i=0;i<16;i++){
  	mcp1.pinMode(i, INPUT); // set all pins on board #1 as inputs
  	mcp1.pullUp(i, HIGH);  // turn on a 100K pullup internally
  	mcp2.pinMode(i, OUTPUT); // set all pins on board #2 as outputs, for LEDs
	}

  pinMode(LED_PIN, OUTPUT);
	digitalWrite(LED_PIN, LOW);
	Serial.println("setup finished");
}

void loop() {
	thisloop_ms=millis();
	loopcount++;
	if (thisloop_ms>=looptime_ms){
		Serial.println((String)"Loops/Sec: " + loopcount);
		loopcount=0;
		looptime_ms=thisloop_ms+1000;
	}

	if (thisloop_ms>nextblinkoff_ms){
		digitalWrite(LED_PIN, LOW);
	}

	uint16_t tmpInt=mcp1.readGPIOAB();
	if (tmpInt!=buttonState) {
		handleButtons(tmpInt);
		digitalWrite(LED_PIN, HIGH);
		nextblinkoff_ms=thisloop_ms+100;
	}
}
