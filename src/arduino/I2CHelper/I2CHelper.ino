#include <FastLED.h>
#include "I2CHelper.h"

#define ADDRESS 0x61
#define NUM_LEDS 64
#define LED_PIN 10

volatile float a = 1.0f, b = 1.0f;
const long interval = 10;
unsigned long previousMillis = 0;
CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(80);
  I2CHelper::onRequest(requestEvent);
  I2CHelper::begin(ADDRESS);
  Serial.begin(115200);
  while (!Serial) {
  }
  Serial.print(F("Waiting for I2C communication. Address is 0x"));
  Serial.println(ADDRESS, HEX);
}

void loop() {
  if (I2CHelper::reader.hasNewData()) {
    byte command = I2CHelper::reader.getByte();
    a = I2CHelper::reader.getFloat();
    b = I2CHelper::reader.getFloat();
    Serial.print(F("Message received: a="));
    Serial.print(a);
    Serial.print(F(", b="));
    Serial.println(b);
  } else {
    //Serial.println("Nothing to report");
  }
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    updateLEDs();
  }

}

void updateLEDs() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].fadeToBlackBy(5);
  }
  CRGB firstLED = leds[0];
  for (int i = 0; i < NUM_LEDS - 1; i++) {
    leds[i] = leds[i + 1];
  }
  leds[NUM_LEDS - 1] = firstLED;
  leds[random(NUM_LEDS)] = CHSV(60 + random(-60, 60), 255, 255);
  FastLED.show();
}

void requestEvent() {
  //Serial.println(F("Request event triggered"));
  a += 1.0f;
  I2CHelper::responder.addFloat(a);
  int bytesSent = I2CHelper::responder.write(false);
}

