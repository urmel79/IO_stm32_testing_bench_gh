/*
 * Blink
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */

#include <Arduino.h>

#include "config.hpp"

#include "function_bme280.hpp"

unsigned long g_ul_loop_previousMillis = 0; // store last execution time of main loop
#define LOOP_DELAY_TIME_MS 2000             // interval for the main loop (milliseconds)

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

}

unsigned long g_ul_led_previousMillis = 0;  // store last execution time of led loop
unsigned long g_ul_led_delay_time_ms = 120; // interval for the led loop (milliseconds)
int g_i_state = 0;   // state of the FSM

// blink the external led like a heartbeat
// pulse mode is realised in an finite state machine (non blocking parallel task)
void blink_led_heartbeat_fsm() {
  // blinking like a heartbeat :)

  // execute the led loop without delay
  unsigned long l_loop_currentMillis = millis();
  if (l_loop_currentMillis - g_ul_led_previousMillis >= g_ul_led_delay_time_ms) {
    // store last execution time of main loop
    g_ul_led_previousMillis = l_loop_currentMillis;

    switch (g_i_state) {
      case 0:
        Serial.println("Onboard LED: ON");
        digitalWrite(ONBOARD_LED, LED_ON);  // turn the LED on
        g_ul_led_delay_time_ms = 120;
        g_i_state++;
        break;
      case 1:
        Serial.println("Onboard LED: OFF");
        digitalWrite(ONBOARD_LED, LED_OFF); // turn the LED off
        g_i_state++;
        break;
      case 2:
        Serial.println("Onboard LED: ON");
        digitalWrite(ONBOARD_LED, LED_ON);  // turn the LED on
        g_i_state++;
        break;
      case 3:
        Serial.println("Onboard LED: OFF");
        digitalWrite(ONBOARD_LED, LED_OFF); // turn the LED off
        g_ul_led_delay_time_ms = 1000;
        g_i_state = 0; // reset state
        break;
      default:
        // never reach this
        break;
    }
  }
}

void loop() {

  blink_led_heartbeat_fsm();

  // execute the main loop without delay
  unsigned long l_loop_currentMillis = millis();
  if (l_loop_currentMillis - g_ul_loop_previousMillis >= LOOP_DELAY_TIME_MS) {
    // store last execution time of main loop
    g_ul_loop_previousMillis = l_loop_currentMillis;

    if (g_b_bme280_iic_connected) {
      g_d_bme280_temperature = bme280_temp_get();
      g_i_bme280_humidity = bme280_hum_get();
      g_d_bme280_pressure = bme280_press_get();
      g_i_bme280_altitude = bme280_alt_get();

      bme280_printSerial(g_d_bme280_temperature, g_i_bme280_humidity, g_d_bme280_pressure, g_i_bme280_altitude);
    }
  }
}


//
