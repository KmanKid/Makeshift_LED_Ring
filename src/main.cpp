#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define PIN D4
#define STRIPSIZE 16

Adafruit_NeoPixel strip = Adafruit_NeoPixel(STRIPSIZE, PIN, NEO_GRB + NEO_KHZ800);
ESP8266WebServer server(80);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

const char *ssid = "ssid";
const char *password = "password";
uint32_t hour_color = strip.Color(255, 0, 255);
uint32_t minute_color = strip.Color(0, 255, 255);

void blink(int speed)
{
    strip.clear();
    strip.show();
    delay(speed / 2);
    strip.setPixelColor(0, strip.Color(255, 255, 255));
    strip.show();
    delay(speed / 2);
}

void setup()
{
    Serial.begin(9600);
    strip.begin();
    strip.setBrightness(25);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        blink(500);
        Serial.print(".");
    }
    timeClient.begin();
    timeClient.setTimeOffset(7200);
}

void setHourLed(int hour)
{
    int led = hour - 12 > 0 ? hour - 12 : hour;
    strip.setPixelColor(led, hour_color);
}

void setMinuteLed(int minute)
{
    int led = floor(minute / 5);
    strip.setPixelColor(led, minute_color);
}

void setLedToTime(int hour = 0, int minute = 0)
{
    strip.clear();
    setMinuteLed(minute);
    setHourLed(hour);
    strip.show();
}

void loop()
{
    delay(1000);
    timeClient.update();
    setLedToTime(timeClient.getHours(), timeClient.getMinutes());
}