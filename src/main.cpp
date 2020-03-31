
#include <Arduino.h>

#include "config.hpp"

#include "function_bme280.hpp"
#include "function_wifi.hpp"
#include "function_webserver.hpp"

unsigned long g_ul_loop_previousMillis = 0; // store last execution time of main loop
#define LOOP_DELAY_TIME_MS 4000             // interval for the main loop (milliseconds)

bool g_b_bme280_iic_connected = false;
double g_d_bme280_temperature;
int g_i_bme280_humidity;
double g_d_bme280_pressure;
int g_i_bme280_altitude;

// IMPORTANT NOTE TO SERIAL NUMBERING!!
//
// Source: https://stm32duinoforum.com/forum/wiki_subdomain/index_title_API.html#Serial_.26_USB_Serial
//
// Serial USB is enabled for all F103 boards when uploading using the bootloader, it is also available when uploading by ST-Link (SWD) In these cases:
// - Serial.print("Hello world"); will print via Serial USB (CDC).
// - Serial1 prints to hardware USART 1
// - Serial2 prints to hardware USART 2
// - etc
//
// When uploading via "Serial" (external USB to Serial adaptor connected to PA9 and PA10 (USART1) on the STM32 Bluepill):
// - Serial.print("Hello world"); will print to hardware USART1 (the one the code was uploaded using)
// - Serial1 prints to hardware USART 2
// - etc
//
// Note. Some boards, e.g. Nucleo F103RB have special serial mapping, because these boards need to have hardware modification to make Serial usable.
// The Serial <-> USART mapping is defined in file "variants//board.cpp".

// with Bluepill we have to create Serial objects first
// Sources:
// https://forum.arduino.cc/index.php?topic=566042.msg3954347#msg3954347
// https://github.com/BLavery/STM32F103-Arduino
HardwareSerial Serial2(PA3, PA2);   // or: HardwareSerial Serial2(USART2);
// HardwareSerial Serial3(PB11, PB10); // or: HardwareSerial Serial3(USART3);


void setup() {
  bool l_b_wifi_connected = false;

  // initialize LED digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  //NOTE: Serial(PA10, PA9) is the serial monitor with baud rate (115200)
  Serial.begin(115200);

  //NOTE: Serial2(PA3, PA2) is connected with ESP8266(RX, TX) respectively with baud rate (115200)
  Serial2.begin(115200);

  g_b_bme280_iic_connected = bme280_iic_connect();

  l_b_wifi_connected = wifi_init();
  if(l_b_wifi_connected) webserver_init();

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
        // Serial.println("Onboard LED: ON");
        digitalWrite(ONBOARD_LED, LED_ON);  // turn the LED on
        g_ul_led_delay_time_ms = 120;
        g_i_state++;  // switch to next state
        break;
      case 1:
        // Serial.println("Onboard LED: OFF");
        digitalWrite(ONBOARD_LED, LED_OFF); // turn the LED off
        g_i_state++;  // switch to next state
        break;
      case 2:
        // Serial.println("Onboard LED: ON");
        digitalWrite(ONBOARD_LED, LED_ON);  // turn the LED on
        g_i_state++;  // switch to next state
        break;
      case 3:
        // Serial.println("Onboard LED: OFF");
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

    webserver_requestHandler();
  }
}


//
