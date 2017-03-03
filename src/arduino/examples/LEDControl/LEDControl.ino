/*
   Shows an example of using the I2CHelper to control a FastLED based light show. This is a
   potentially tricky case on the ATMega324 as FastLED jumps all over the interrupts, making
   I2C unreliable. The 'correct' solution in this case is to use somethign else, either a
   different LED chip (this example uses neopixels) or a faster microcontroller (where the
   FastLED library can re-enable interrupts between pixels and avoid the issues) but this
   should work based on the error correction and retry logic in the I2CHelper library.
*/

#include <I2CHelper.h>
#include <FastLED.h>
#include "Interval.h"

/*
   Configure the example.

   ADDRESS is the I2C address on which we listen for commands.
   NUM_LEDS is the number of neopixels attached to the data pin.
   LED_PIN is the hardware pin used to send data to the neopixel strip.
*/
#define ADDRESS 0x61
#define NUM_LEDS 64
#define LED_PIN 10

/*
   Volatile state, the hue and hue_variation are used to configure the light
   show, the Interval instance is used to determine how often it should be
   updated and the CRGB[] contains the actual LED colour data.
*/
byte hue = 0;
byte hue_variation = 30;
Interval ledUpdate(10);
CRGB leds[NUM_LEDS];

/*
   Setup - starts up the I2CHelper and Serial connections

   Note that this waits for a serial monitor to be started, so you won't see any lights flashing
   until you connect the monitor.
*/
void setup() {
  Serial.begin(115200);
  while (!Serial) {
  }
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(80);
  I2CHelper::begin(ADDRESS);
}

/*
   Each time round the loop we check to see whether the I2CReader has new data. If it has, then read
   off the first byte and use that to interpret the command, reading subsequent data if needed based
   on the command code.

   Update the LEDs if the interval between updates has expired.
*/
void loop() {
  if (I2CHelper::reader.hasNewData()) {
    if (I2CHelper::reader.checksumValid()) {
      Serial.println("New data!");
      byte command = I2CHelper::reader.getByte();
      Serial.print("Command received: ");
      Serial.println(command, DEC);
      switch (command) {
        case 1:
          hue = I2CHelper::reader.getByte();
          hue_variation = I2CHelper::reader.getByte();
          break;
        case 2:
          FastLED.setBrightness(I2CHelper::reader.getByte());
        default:
          break;
      }
    }
    else {
      I2CHelper::reader.start();
    }
  }
  I2CHelper::reader.printBuffer();
  if (ledUpdate.shouldRun()) {
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i].fadeToBlackBy(5);
    }
    CRGB firstLED = leds[0];
    for (int i = 0; i < NUM_LEDS - 1; i++) {
      leds[i] = leds[i + 1];
    }
    leds[NUM_LEDS - 1] = firstLED;
    leds[random(NUM_LEDS)] = CHSV(hue + random(-hue_variation, hue_variation), 255, 255);
    FastLED.show();
  }
}

