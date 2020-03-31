#include "function_wifi.hpp"

#include "config.hpp"
#include "wifi_creds.hpp"

#define MAX_DELAY_MS  8000
#define MAX_TRIALS    8

//NOTE: Serial(PA10, PA9) is the serial monitor with baud rate (115200)
//NOTE: Serial2(PA3, PA2) is connected with ESP8266(RX, TX) respectively with baud rate (115200)

bool wifi_check4IP(unsigned long timeout) { // function to check ip of ESP8266
  unsigned long l_ul_curr_time = millis();
  bool l_b_found_expr = false;

  while(l_ul_curr_time+timeout > millis()) {   // search with defined timeout
    while(Serial2.available() > 0) {
      if(Serial2.find("WIFI GOT IP")) l_b_found_expr = true;
    }
  }

  return l_b_found_expr;
}

String wifi_get_ip(uint8_t trials) {              // function retrieves the IP address
  String l_str_IP = "";
  char l_c_buff = 0;

  // validate max trials
  if (trials > MAX_TRIALS) trials = MAX_TRIALS;

  for(uint8_t i=1; i<=trials; i++) {
    Serial2.println("AT+CIFSR");         // sends IP-get command to ESP8266 via serial communication
    while(Serial2.available()) {
      if(Serial2.find("STAIP,\"")) {       // This finds the STAIP that is the STATIC IP ADDRESS of ESP8266
        delay(200);
        while(Serial2.available()>0) {
          l_c_buff = Serial2.read();              // Serial2 reads from ESP8266
          if(l_c_buff == '\"') break;              // stop reading if '\"' occurs
          l_str_IP += l_c_buff;
        }

        i = trials;  // leave loop
      }
      // if(l_c_buff == '+') break;              // stop reading if '+' occurs
    }

    delay(100);                              // @TODO: kill evil delays!
  }

  return l_str_IP;
}

String wifi_get_mac(uint8_t trials) {              // function retrieves the MAC address
  String l_str_MAC = "";
  char l_c_buff = 0;

  // validate max trials
  if (trials > MAX_TRIALS) trials = MAX_TRIALS;

  for(uint8_t i=1; i<=trials; i++) {
    Serial2.println("AT+CIFSR");         // sends IP-get command to ESP8266 via serial communication
    while(Serial2.available()) {
      if(Serial2.find("STAMAC,\"")) {       // This finds the STAIP that is the STATIC IP ADDRESS of ESP8266
        delay(200);
        while(Serial2.available()>0) {
          l_c_buff = Serial2.read();              // Serial2 reads from ESP8266
          if(l_c_buff == '\"') break;              // stop reading if '\"' occurs
          l_str_MAC += l_c_buff;
        }

        i = trials;  // leave loop
      }
      // if(l_c_buff == '+') break;              // stop reading if '+' occurs
    }

    delay(100);                              // @TODO: kill evil delays!
  }

  return l_str_MAC;
}

bool wifi_command(String cmd, int delay_ms, uint8_t trials) {    //This function is for connecting ESP8266 with wifi network by using AT commands
  bool l_b_ok = false;

  // validate delay time
  if (delay_ms > MAX_DELAY_MS) delay_ms = MAX_DELAY_MS;

  // validate max trials
  if (trials > MAX_TRIALS) trials = MAX_TRIALS;

  for(uint8_t i=1; i<=trials; i++) {
    Serial.println(cmd);                  // sends command to serial monitor (logging)
    Serial2.println(cmd);                 // sends command to ESP8266 via serial communication
    while(Serial2.available()) {
      if(Serial2.find("OK")) {
        i = trials;  // leave loop
        l_b_ok = true;
      }
    }

    delay(delay_ms);                    // @TODO: kill evil delays!
  }

  if(l_b_ok)
    Serial.println("OK");
  else
    Serial.println("Error");

  return l_b_ok;
}

bool wifi_connect(String cmd, int delay_ms, uint8_t trials) {    //This function is for connecting ESP8266 with wifi network by using AT commands
  bool l_b_ok = false;

  // validate delay time
  if (delay_ms > MAX_DELAY_MS) delay_ms = MAX_DELAY_MS;

  // validate max trials
  if (trials > MAX_TRIALS) trials = MAX_TRIALS;

  for(uint8_t i=1; i<=trials; i++) {
    Serial.println(cmd);                  // sends command to serial monitor (logging)
    Serial2.println(cmd);                 // sends command to ESP8266 via serial communication
    while(Serial2.available()) {
      if(Serial2.find("WIFI GOT IP")) {
        i = trials;  // leave loop
        l_b_ok = true;
      }
    }

    delay(delay_ms);                    // @TODO: kill evil delays!
  }

  if(l_b_ok)
    Serial.println("OK");
  else
    Serial.println("Error");

  return l_b_ok;
}

bool wifi_init() {                          //This function contains AT commands that passes to wifi_connect()
  // bool l_b_have_ip = false;
  bool l_b_ok = false;
  String l_str_wifi_config = "";

  wifi_command("AT", 500, 5);                   //Sends AT command with time(Command for Achknowledgement)
  wifi_command("AT+CWMODE=3", 500, 5);          //Sends AT command with time (For setting mode of Wifi)
  wifi_command("AT+CWQAP", 500, 5);             //Sends AT command with time (for Quit AP)
  wifi_command("AT+RST", 4000, 5);              //Sends AT command with time (For RESETTING WIFI); wait minimal 2000 ms for reset finished

  l_b_ok = wifi_check4IP(3000);

  if(!l_b_ok) {

    Serial.println("Connecting Wifi....");

    // @TODO:
    //  setting the hostname does not work in the moment ...
    wifi_command("AT+CWMODE=3", 500, 5);          //Sends AT command with time (For setting mode of Wifi)
    l_str_wifi_config = "AT+CWHOSTNAME=\"" + String(HOSTNAME) + "\"";
    wifi_connect(l_str_wifi_config, 1000, 3);     // sends AT command with time (for setting hostname)

    l_str_wifi_config = "AT+CWJAP=\"" + String(WIFI_SSID) + "\",\"" + String(WIFI_PASSWORD) + "\"";
    l_b_ok = wifi_connect(l_str_wifi_config, 2000, 5);     // sends AT command with time (for connecting with WiFi SSID and password)
  }

  if(l_b_ok) {
    Serial.println("Wifi Connected :)");

    Serial.print("IP Address: ");
    Serial.println(wifi_get_ip(3));

    Serial.print("MAC Address: ");
    Serial.println(wifi_get_mac(3));
  }
  else {
    Serial.println("Wifi Error :(");
  }

  return l_b_ok;
}













//
