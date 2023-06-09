#include <DHTesp.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <Wifi.h>
#include <esp_sleep.h>

#define WLAN_SSID       "A1"
#define WLAN_PASS       "88889999"

#define AIO_SERVER      "io.adafruit.com"
#define AIO_USERNAME    "fong1668vn"
#define AIO_KEY         "aio_KiOV99ety8rIv4XRMDtB7ui1UnkD" 
#define AIO_SERVERPORT  1883

#define DHTPIN          4
#define REDPIN          15
#define GREENPIN        2

// Khai báo địa chỉ I2C của màn hình LCD
#define I2C_ADDR        0x27
#define LCD_COLS        16
#define LCD_ROWS        2

DHTesp dht;
WiFiClient client;
LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLS, LCD_ROWS);

/* Config Adafruit */
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Publish Temperature1 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/data-sensor.weather-temperature");
Adafruit_MQTT_Publish Humidity1 = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/data-sensor.weather-humidity");

const int lightPin = 34;
const int soundPin = 35;
const int gasPin = 25;




