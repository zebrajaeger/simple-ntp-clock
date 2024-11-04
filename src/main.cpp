#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <NTPClient.h>
#include <WiFiManager.h>

#include "config.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET, 100000, 100000);

WiFiManager wifiManager;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 60000);

uint8_t x_buffer[16];
uint8_t x_position = 0;

void setup() {
  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Display failed"));
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("WiFi verbinden...");
  display.display();

  wifiManager.autoConnect("ESP-Uhr");

  configTime(timeZone.c_str(), "pool.ntp.org", "time.nist.gov");
  setenv("TZ", timeZone.c_str(), 1);
  tzset();

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Connected!");
  display.display();
  delay(1000);
}

void loop() {
  timeClient.update();

  struct tm timeInfo;
  getLocalTime(&timeInfo);
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.printf("%02d:%02d:%02d\n", timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
  display.setCursor(0, 24);
  display.setTextSize(1);
  display.printf("%02d-%02d-%04d", timeInfo.tm_mday, timeInfo.tm_mon + 1, timeInfo.tm_year + 1900);

  display.display();

  delay(500);
}
