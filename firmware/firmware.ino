// https://learn.adafruit.com/dht/overview

#include "Adafruit_LEDBackpack.h"
#include <Adafruit_GFX.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#define DHTPIN 13
#define DHTTYPE DHT22
#define DISPLAY_FAHRENHEIT true

Adafruit_AlphaNum4 tempDisplay = Adafruit_AlphaNum4();
Adafruit_AlphaNum4 humidDisplay = Adafruit_AlphaNum4();
DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS;

float cToF(float celciusTemp) {
  return (celciusTemp * 1.8) + 32;
}

void setup() {
  Serial.begin(9600);
  dht.begin();

  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);
  delayMS = sensor.min_delay / 1000;

  tempDisplay.begin(0x72);
  humidDisplay.begin(0x70);
}

void printToDisplay(Adafruit_AlphaNum4 display, String string) {
  display.writeDigitAscii(0, string[0]);
  display.writeDigitAscii(1, string[1], true);
  display.writeDigitAscii(2, string[3]);
  display.writeDigitAscii(3, string[4]);

  display.writeDisplay();
}

void loop() {
  delay(delayMS);

  sensors_event_t event;
  dht.temperature().getEvent(&event);

  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
    printToDisplay(tempDisplay, String(0));
  } else {
    Serial.print(F("Temperature: "));
    Serial.print(String(event.temperature));
    Serial.println(F("°C"));
    if (DISPLAY_FAHRENHEIT) {
      printToDisplay(tempDisplay, String(cToF(event.temperature)));
    } else {
      printToDisplay(tempDisplay, String(event.temperature));
    }
  }

  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
    printToDisplay(humidDisplay, String(0));
  } else {
    Serial.print(F("Humidity: "));
    Serial.print(String(event.relative_humidity));
    Serial.println(F("%"));
    printToDisplay(humidDisplay, String(event.relative_humidity));
  }

  char c = Serial.read();
  if (!isprint(c)) return; // only printable!

  delay(200);
}
