#include "function_webserver.hpp"

#include "function_wifi.hpp"

String str_site_style = "<style>\n body { font-family: Helvetica, Arial, Geneva, sans-serif; }\n</style>";
String str_site_name = "<h2>urmel79/IO_esp8266_Neopixel_PIR_Halloween_gh</h2>";   //String with html notations
String str_site_description = "<p>This project is an example application for ESP8266, a Styrofoam pumpkin <br>illuminated by Neopixel LED ring and a mp3 sound output based on DFPlayer.</p>";
String str_site_data = "<p>Data Received Successfully ...</p>";     //String with html

void webserver_init() {
  wifi_command("AT+CIPMUX=1", 100, 5);                          //Sends AT command with time (For creating multiple connections)
  wifi_command("AT+CIPSERVER=1,80", 100, 5);                    //Sends AT command with time (For setting up server with port 80)

  Serial.print("Webserver port: ");
  Serial.println(80);
}

void webserver_sendwebdata(String webPage) {  //This function is used to send webpage datas to the localserver
  int ii=0;

  while(1) {
    unsigned int l=webPage.length();
    Serial.print("AT+CIPSEND=0,");
    Serial2.print("AT+CIPSEND=0,");
    Serial.println(l+2);
    Serial2.println(l+2);
    delay(100);
    Serial.println(webPage);                        //sends webpage data to serial monitor
    Serial2.println(webPage);                       //sends webpage data to serial2 ESP8266
    delay(20);                       // sending over serial line needs some time ... so be patient
    while(Serial2.available()) {
      if(Serial2.find("OK")) {
        ii=11;
        break;
      }
    }
    if(ii==11) break;
    delay(100);
  }
}

void webserver_updatePage() {  //This function contains data to be sent to local server
  String l_str_htmlData = "";           //String variable to store characters

  l_str_htmlData = "<html>\n<head>";
  webserver_sendwebdata(l_str_htmlData);

  l_str_htmlData = str_site_style;
  webserver_sendwebdata(l_str_htmlData);

  l_str_htmlData = "<title>My STM32 test page</title>\n</head>\n<body bgcolor=ccffff>";
  webserver_sendwebdata(l_str_htmlData);

  l_str_htmlData = "<h1>Welcome to Bjoerns STM32 test page</h1>";
  webserver_sendwebdata(l_str_htmlData);

  l_str_htmlData = str_site_name;
  webserver_sendwebdata(l_str_htmlData);

  // l_str_htmlData = str_site_description;
  // webserver_sendwebdata(l_str_htmlData);

  // l_str_htmlData = "<a href=\"https://github.com/urmel79/IO_esp8266_Neopixel_PIR_Halloween_gh/\">Click Here to get into the project at github.</a>";
  // webserver_sendwebdata(l_str_htmlData);

  l_str_htmlData = str_site_data;
  webserver_sendwebdata(l_str_htmlData);

  unsigned long t_runSecondsTotal = millis() / 1000;
  int t_runHours = t_runSecondsTotal / 3600;
  int t_secsRemaining = t_runSecondsTotal % 3600;
  int t_runMinutes = t_secsRemaining / 60;
  int t_runSeconds = t_secsRemaining % 60;
  char str_timeString[21];
  sprintf(str_timeString,"%02d:%02d:%02d", t_runHours, t_runMinutes, t_runSeconds);

  l_str_htmlData = "<p>Time since last reboot [hh:mm:ss]: <b>";
  l_str_htmlData+= str_timeString;
  l_str_htmlData+= "</b></p>";
  webserver_sendwebdata(l_str_htmlData);

  l_str_htmlData = "</body>\n</html>";
  webserver_sendwebdata(l_str_htmlData);

  Serial2.println("AT+CIPCLOSE=0");                  //Closes the server connection
}

void webserver_requestHandler() {

  Serial.println("Please Refresh your Page");

  while(Serial2.available()) {
    if(Serial2.find("0,CONNECT")) {
      Serial.println("Start Printing");
      webserver_updatePage();
      Serial.println("Done Printing");
      delay(1000);
    }
  }
}







//
