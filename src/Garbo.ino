#include <Bridge.h>
#include <Process.h>

#include "./sampling.h"
#include "./utils.h"
#include "./dht.h"

#define DHT_DEBUG

extern Adafruit_TSL2561_Unified tsl;
extern Adafruit_BMP085_Unified bmp;
extern DHT dht;

Process p;

void setup() {
  Serial.begin(9600);
  Bridge.begin();

  DEBUG_PRINTLN("Checking connectivity... ");
  while (!isConnectedToInternet(&p));
  DEBUG_PRINTLN("Connected");

  DEBUG_PRINTLN("Syncing clock...");
  setClock(&p);

  if (bmp.begin()) {
    DEBUG_PRINTLN("BMP180 init success");
  } else {
    DEBUG_PRINTLN("BMP180 init failed");
    while(1);
  }

  dht.begin();
  
  // Initialize light sensor
  tsl.enableAutoRange(true);
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);
  DEBUG_PRINTLN("TSL2561 init success");
}

void loop() {
  WeatherSample sample;
  sampleWeather(&sample);
  
  p.begin("curl");
  p.addParameter("-i");
  p.addParameter("-XPOST");
  p.addParameter("http://192.168.178.37:8086/write?db=garbo");
  p.addParameter("-d "
    "humidity,node=pluto value=" + String(sample.humidity) + "\n"
    "temperature,node=pluto value=" + String(sample.temperature) + "\n"
    "pressure,node=pluto value=" + String(sample.pressure) + "\n"
    "light,node=pluto value=" + String(sample.light) + "\n"
  );
  p.run();

  delay(10000);
}

