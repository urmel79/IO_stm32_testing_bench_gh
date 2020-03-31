#ifndef function_bme280_hpp
#define function_bme280_hpp

// Libs for BME280 sensor
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

/*
 * Nucleo64 F103RB: I2C bus uses pins:
 * D14 = SDA
 * D15 = SCL
 *
 * Bluepill F103C8 (128kB): I2C bus uses pins:
 * PB7 = SDA1
 * PB6 = SCL1
 *
 * Caution: does not work! Had fried my BME280 sensor :(
 * PB11 = SDA2
 * PB10 = SCL2
*/
#if defined(NUCLEO_F103RB)
  const int sdaPin = D14;
  const int sclPin = D15;
#elif defined(BLUEPILL_F103C8)
  const int sdaPin = PB7;
  const int sclPin = PB6;
  // const int sdaPin = PB11;
  // const int sclPin = PB10;
#endif

// mittlerer Luftdruck der Atmosphäre (atmosphärischer Druck) auf Meereshöhe
// #define SEALEVELPRESSURE_HPA (1013.25)
// #define SEALEVELPRESSURE_HPA (1015) // DD-Gittersee, 2019-08-31 14:00
// #define SEALEVELPRESSURE_HPA (1015) // Nürnberg, 2019-08-28 20:00
// #define SEALEVELPRESSURE_HPA (1019) // Nürnberg, 2019-08-29 12:00
// #define SEALEVELPRESSURE_HPA (1022) // DD-Albertstadt, 2019-08-30 10:00
// #define SEALEVELPRESSURE_HPA (1026) // DD-Albertstadt, 2020-03-18 15:00
// #define SEALEVELPRESSURE_HPA (1025) // DD-Albertstadt, 2020-03-19 11:00
// #define SEALEVELPRESSURE_HPA (1024) // DD-Albertstadt, 2020-03-19 17:00
// #define SEALEVELPRESSURE_HPA (1023) // DD-Albertstadt, 2020-03-20 16:00
// #define SEALEVELPRESSURE_HPA (1028) // DD-Gittersee, 2020-03-30 14:00
#define SEALEVELPRESSURE_HPA (1030) // DD-Gittersee, 2020-03-31 09:00

bool bme280_iic_connect();

double bme280_temp_get();
int bme280_hum_get();
double bme280_press_get();
int bme280_alt_get();

void bme280_printSerial(double temp, int hum, double press, int alt);

#endif // function_bme280_hpp







//
