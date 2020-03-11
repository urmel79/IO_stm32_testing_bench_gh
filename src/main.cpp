/*
 * Blink
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */

#include <Arduino.h>

#include "config.hpp"

// blink onboard like heartbeat
#define delay_ms_on 1000 // 1000 ms on delay
#define delay_ms_off 120 // 120 ms off delay

void setup() {
  // initialize LED digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  // initialize serial communication at 57600 bits per second:
  Serial.begin(115200);

  // //activate USB CDC driver
	// SerialUSB.begin(9600);
}

void loop() {
  // blinking like a heartbeat :)
  delay(delay_ms_on);               // wait to switch led on
  Serial.println("Onboard LED: ON");
  digitalWrite(ONBOARD_LED, LOW);   // turn the LED on (negative logic)
  delay(delay_ms_off);              // wait to switch led off
  Serial.println("Onboard LED: OFF");
  digitalWrite(ONBOARD_LED, HIGH);  // turn the LED off (negative logic)
  delay(delay_ms_off);              // wait to switch led off
  Serial.println("Onboard LED: ON");
  digitalWrite(ONBOARD_LED, LOW);   // turn the LED on (negative logic)
  delay(delay_ms_off);              // wait to switch led off
  Serial.println("Onboard LED: OFF");
  digitalWrite(ONBOARD_LED, HIGH);  // turn the LED off (negative logic)
}


//
