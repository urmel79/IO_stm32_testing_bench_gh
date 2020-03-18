#include "function_bme280.hpp"

// set to fixed value (no wifi connection to OpenWeatherMaps possible at the moment)
int g_i_sealevel_pressure_nn = SEALEVELPRESSURE_HPA;

Adafruit_BME280 bme; // I2C

bool bme280_iic_connect() {
  bool status;

//   //Initialize BME280
//   bme.settings.runMode = 3; //Normal mode
//   bme.settings.tStandby = 0;
//   bme.settings.filter = 4;
//   bme.settings.tempOverSample = 5;
//   bme.settings.pressOverSample = 5;
//   bme.settings.humidOverSample = 5;

  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  Wire.begin(sdaPin, sclPin);
  status = bme.begin(0x76, &Wire);  // 0x76: default i2c address of bme280
  if (!status) Serial.println("Could not find a valid BME280 sensor, check wiring!");

  return status;
}

double bme280_temp_get() {
  return bme.readTemperature();
}

int bme280_hum_get() {
  return int(bme.readHumidity());
}

double bme280_press_get() {
  return bme.readPressure() / 100.0F; // 100.0F ... type cast to float to force floating point division
}

int bme280_alt_get() {
  // erst Höhe berechnen, wenn Luftdruck NN von OpenWeatherMaps bezogen wurde
  if (g_i_sealevel_pressure_nn != 0) {
    return int(bme.readAltitude(g_i_sealevel_pressure_nn));
  }
  else return -1;
}

void bme280_printSerial(double temp, int hum, double press, int alt) {
  Serial.print("Temperature = ");
  Serial.print(temp, 2);
  Serial.println(" °C");

  // Convert temperature to Fahrenheit
  //Serial.print("Temperature = ");
  //Serial.print(1.8 * bme.readTemperature() + 32);
  //Serial.println(" *F");

  Serial.print("Humidity = ");
  Serial.print(hum);
  Serial.println(" % rH");

  Serial.print("Pressure = ");
  Serial.print(press);
  Serial.println(" hPa");

  Serial.print("Approx. Altitude = ");
  Serial.print(alt);
  Serial.println(" m");

  Serial.println();
}















//
