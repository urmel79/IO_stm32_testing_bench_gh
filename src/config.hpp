#ifndef config_hpp
#define config_hpp

#ifdef INTERNAL_LED_PC13
  #define ONBOARD_LED PC13
#elif INTERNAL_LED_P33
  #define ONBOARD_LED 33
#elif INTERNAL_LED_D13
  #define ONBOARD_LED D13
#endif

#ifdef NEGATIVE_OUTPUT_LOGIC
  #define LED_ON LOW
  #define LED_OFF HIGH
#else
  #define LED_ON HIGH
  #define LED_OFF LOW
#endif


#endif // config_hpp
