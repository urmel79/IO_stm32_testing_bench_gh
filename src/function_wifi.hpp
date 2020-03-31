#ifndef function_wifi_hpp
#define function_wifi_hpp

#include <Arduino.h>


bool wifi_check4IP(unsigned long timeout);

String wifi_get_ip(uint8_t trials);
String wifi_get_mac(uint8_t trials);

bool wifi_command(String cmd, int delay_ms, uint8_t trials);
bool wifi_connect(String cmd, int delay_ms, uint8_t trials);

bool wifi_init();



#endif // function_wifi_hpp
