// Demo the quad alphanumeric display LED backpack kit
// scrolls through every character, then scrolls Serial
// input onto the display

#include "Adafruit_LEDBackpack.h"
#include <Adafruit_GFX.h>
#include <DHT.h>
#include <Wire.h>

#define DHTPIN 13
#define DHTTYPE DHT22
#define DISPLAY_FAHRENHEIT true

Adafruit_AlphaNum4 tempDisplay = Adafruit_AlphaNum4();
Adafruit_AlphaNum4 humidDisplay = Adafruit_AlphaNum4();
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();

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
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  String humidity = String(h);

  float t = dht.readTemperature(DISPLAY_FAHRENHEIT);
  String temperature = String(t);

  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return; // TODO display err on alphanumeric display
  }

  char c = Serial.read();
  if (!isprint(c))
    return; // only printable!

  printToDisplay(humidDisplay, humidity);
  printToDisplay(tempDisplay, temperature);

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));

  delay(200);
}
