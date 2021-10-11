Simple project to display temperature and humidity readouts from a DHT22 sensor to alphanumeric LED displays.

Temperature is displayed in Fahrenheit. Change [`DISPLAY_FAHRENHEIT` on line 12](https://github.com/Nase00/ambient-temp-monitor/blob/master/firmware/firmware.ino#L12) to `false` to display Celcius.

# Hardware

- [Quad Alphanumeric Display - Red 0.54" Digits w/ I2C Backpack](https://www.adafruit.com/product/1911) x2 (Recommended red, yellow, or orange for Temperature, blue for Humidity)

- [DHT22](https://www.adafruit.com/product/385) x1

- Arduino, ESP32, or ESP8826

### `firmware/secrets.h`

Create the following file with your secret WiFi credentials:

```c
const char *SSID = "ithurtswhenip";
const char *PASSWORD = "hunter";
```
