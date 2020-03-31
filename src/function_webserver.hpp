#ifndef function_webserver_hpp
#define function_webserver_hpp

#include <Arduino.h>

void webserver_init();
void webserver_sendwebdata(String webPage);
void webserver_updatePage();
void webserver_requestHandler();


#endif // function_webserver_hpp
