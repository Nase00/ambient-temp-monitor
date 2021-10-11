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

// You must create this file yourself, see README.md
#include "secrets.h"

#define DHTPIN 13
#define DHTTYPE DHT22
#define DISPLAY_FAHRENHEIT true

ESP8266WebServer server(80);

Adafruit_AlphaNum4 tempDisplay = Adafruit_AlphaNum4();
Adafruit_AlphaNum4 humidDisplay = Adafruit_AlphaNum4();
DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS;
float temperature;
float humidity;

float cToF(float celciusTemp) {
  return (celciusTemp * 1.8) + 32;
}

void printToDisplay(Adafruit_AlphaNum4 display, String string) {
  display.writeDigitAscii(0, string[0]);
  display.writeDigitAscii(1, string[1], true);
  display.writeDigitAscii(2, string[3]);
  display.writeDigitAscii(3, string[4]);

  display.writeDisplay();
}

void handleRoot() {
  server.send(200, "application/json", "{\"temperature\":" + String(temperature) + ",\"humidity\":" + String(humidity) + "}");
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void processWriteTempHumidity() {
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
      temperature = cToF(event.temperature);
    } else {
      printToDisplay(tempDisplay, String(event.temperature));
      temperature = event.temperature;
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
    humidity = event.relative_humidity;
  }
}

void setup() {
  Serial.begin(9600);
  dht.begin();

  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);
  delayMS = sensor.min_delay / 500;

  tempDisplay.begin(0x72);
  humidDisplay.begin(0x70);

  // Render once before WiFi starts connecting
  processWriteTempHumidity();

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(SSID);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);

  server.begin();
}

void loop() {
  server.handleClient();
  MDNS.update();

  processWriteTempHumidity();

  char c = Serial.read();
  if (!isprint(c)) return; // only printable!

  delay(delayMS);
}
