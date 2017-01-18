#ifndef SAMPLING_H
#define SAMPLING_H


#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>
#include <Adafruit_TSL2561_U.h>
#include "./dht.h"


Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10180);
DHT dht = DHT(5, DHT11);


typedef struct  WeatherSample {
  float         humidity;
  float         temperature;
  float         pressure;
  float         light;
}               WeatherSample;

void  sampleWeather(WeatherSample* sample) {
  sensors_event_t event;
  
  bmp.getEvent(&event);
  if (event.pressure) {
    sample->pressure = event.pressure;
  }

  tsl.getEvent(&event);
  if (event.light) {
    sample->light = event.light;
  }

  sample->humidity = dht.readHumidity();
  sample->temperature = dht.readTemperature();
}

#endif
