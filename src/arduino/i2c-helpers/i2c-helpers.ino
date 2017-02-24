#include <FastLED.h>
#include <Wire.h>
#include "I2CHelper.h"

#define ADDRESS 0x70
#define BUFFER_SIZE 30

byte receiveBuffer[BUFFER_SIZE];
byte sendBuffer[BUFFER_SIZE];

volatile float a, b;


I2CResponder responder = I2CResponder(sendBuffer, BUFFER_SIZE);
I2CReader reader = I2CReader(receiveBuffer, BUFFER_SIZE);

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif

#define NUM_LEDS 16
const long interval = 20;
unsigned long previousMillis = 0;
CRGB leds[NUM_LEDS];
volatile boolean hasData = false;

void setup() {
  FastLED.addLeds<NEOPIXEL, 6>(leds, NUM_LEDS);
  FastLED.setBrightness(60);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);
  Wire.begin(ADDRESS);
  Serial.begin(115200);
  while (!Serial) {
  }
  Serial.print(F("Waiting for I2C communication. Address is 0x"));
  Serial.println(ADDRESS, HEX);
  a = 1.23f;
  b = 5.67f;
}

void loop() {
  if (reader.hasNewData()) {
    //byte command = reader.getByte();
    a = reader.getFloat();
    //b = reader.getFloat();
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
    leds[i].fadeToBlackBy(15);
  }
  leds[random(16)] = CHSV(60 + random(-30, 30), 255, 255);
  FastLED.show();
}

void requestEvent() {
  //Serial.println(F("Request event triggered"));
  float c = a * b;
  responder.start();
  responder.addFloat(c);
  int bytesSent = responder.write(false);
}

void receiveEvent(int dataSize) {
  if (dataSize == 0) {
    //Serial.println(F("Received SMBus probe"));
  } else {
    reader.receiveByte();
  }
}

