#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <WiFiManager.h>

#define PIN D4
#define STRIPSIZE 12

enum COLOR
{
    RED,
    GREEN,
    BLUE,
    PURPLE,
    MINUTE_COLOR
};

Adafruit_NeoPixel strip = Adafruit_NeoPixel(STRIPSIZE, PIN, NEO_GRB + NEO_KHZ800);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

int global_led_offset = 6;

int getLed(int led)
{
    int offset_led = (led + global_led_offset) - 12 > 0 ? (led + global_led_offset) - 12 : (led + global_led_offset);
    return offset_led;
}

int getLedColor(COLOR color, int intesity)
{
    switch (color)
    {
    case RED:
        return strip.Color(intesity, 0, 0);
        break;
    case GREEN:
        return strip.Color(0, intesity, 0);
        break;
    case BLUE:
        return strip.Color(0, 0, intesity);
        break;
    case PURPLE:
        return strip.Color(intesity, 0, intesity);
        break;
    case MINUTE_COLOR:
        return strip.Color(0, intesity, 255);
        break;
    default:
        return strip.Color(intesity, intesity, intesity);
        break;
    }
}

void setup()
{
    Serial.begin(9600);
    strip.begin();
    strip.setBrightness(255);
    WiFiManager wifiManager;
    wifiManager.autoConnect("AP-Name", "<password>");
    timeClient.begin();
    timeClient.setTimeOffset(7200);
}

int setHourLed(int hour)
{
    int led = getLed(hour - 12 > 0 ? hour - 12 : hour);
    strip.setPixelColor(led, getLedColor(RED, 255));
    return led;
}

int setMinuteLed(int minute)
{
    int led = getLed(floor(minute / 5));
    int intensity = ((minute % 5) + 1) * 51;
    strip.setPixelColor(led, getLedColor(MINUTE_COLOR, intensity));
    return led;
}

void setLedToTime(int hour = 0, int minute = 0)
{
    strip.clear();
    int minute_led = setMinuteLed(minute);
    int hour_led = setHourLed(hour);
    if (hour_led == minute_led)
    {
        strip.clear();
        strip.setPixelColor(hour_led, getLedColor(PURPLE, 255));
    }
    strip.show();
}

void loop()
{
    delay(1000);
    timeClient.update();
    setLedToTime(timeClient.getHours(), timeClient.getMinutes());
}