/*
 * Blink
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */

#include <Arduino.h>

#include "config.hpp"

#include "function_bme280.hpp"

// blink onboard like heartbeat
// #define delay_ms_on 1500 // 1000 ms on delay
// #define delay_ms_off 200 // 120 ms off delay
#define delay_ms_on 1000 // 1000 ms on delay
#define delay_ms_off 120 // 120 ms off delay

bool g_b_bme280_iic_connected = false;
double g_d_bme280_temperature;
int g_i_bme280_humidity;
double g_d_bme280_pressure;
int g_i_bme280_altitude;

void setup() {
  // initialize LED digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);

  g_b_bme280_iic_connected = bme280_iic_connect();

  // //activate USB CDC driver
	// SerialUSB.begin(9600);
}

void loop() {
  // blinking like a heartbeat :)
  delay(delay_ms_on);                 // wait to switch led on
  Serial.println("Onboard LED: ON");
  digitalWrite(ONBOARD_LED, LED_ON);  // turn the LED on
  delay(delay_ms_off);                // wait to switch led off
  Serial.println("Onboard LED: OFF");
  digitalWrite(ONBOARD_LED, LED_OFF); // turn the LED off
  delay(delay_ms_off);                // wait to switch led off
  Serial.println("Onboard LED: ON");
  digitalWrite(ONBOARD_LED, LED_ON);  // turn the LED on
  delay(delay_ms_off);                // wait to switch led off
  Serial.println("Onboard LED: OFF");
  digitalWrite(ONBOARD_LED, LED_OFF); // turn the LED off

  if (g_b_bme280_iic_connected) {
    g_d_bme280_temperature = bme280_temp_get();
    g_i_bme280_humidity = bme280_hum_get();
    g_d_bme280_pressure = bme280_press_get();
    g_i_bme280_altitude = bme280_alt_get();

    bme280_printSerial(g_d_bme280_temperature, g_i_bme280_humidity, g_d_bme280_pressure, g_i_bme280_altitude);
  }
}


//
