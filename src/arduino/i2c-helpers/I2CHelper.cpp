/*
 * I2CHelper.cpp - Library for handling unreliable I2C communication 
 * with a Raspbery Pi. 
 * 
 * Created by Tom Oinn, February 20, 2017
 * 
 * Released under the ASL2
 */

#include <Arduino.h>
#include <Wire.h>
#include "I2CHelper.h"

volatile union floatUnion {
  float f;
  byte fBuff[sizeof(float)];
} fu;

I2CResponder::I2CResponder(byte* b, int bs) {
  buffer = b;
  bufferSize = bs;
  writePosition = 0;
  dataValid = true;
  checksum = 0;
}

void I2CResponder::start() {
  writePosition = 0;
  dataValid = true;
  checksum = 0;
}

void I2CResponder::addByte(byte b) {
  if (writePosition < (bufferSize - 1)) {
    buffer[writePosition++] = b;
  } else {
    dataValid = false;
  }
  checksum ^= b;
}

void I2CResponder::addFloat(float f) {
  fu.f = f;
  for (int i = 0; i < sizeof(float); i++) {
    addByte(fu.fBuff[i]);
  }
}

int I2CResponder::write(boolean addChecksum) {
  if (addChecksum) {
    addByte(checksum);
  }
  if (dataValid) {
    int sentBytes = Wire.write(buffer, writePosition);
  }
  else {
    return -1;
  }
}

I2CReader::I2CReader(byte* b, int bs) {
  buffer = b;
  bufferSize = bs;
  readPosition = 0;
  newData = false;
}

void I2CReader::start() {
  readPosition = 0;
}

byte I2CReader::getByte() {
  if (readPosition < bufferSize) {
    return buffer[readPosition++];
  } else {
    return 0;
  }
}

float I2CReader::getFloat() {
  for (int i = 0; i < sizeof(float); i++) {
    fu.fBuff[i] = getByte();
  }
  return fu.f;
}

void I2CReader::receiveByte() {
  long now = millis();
  if (now - lastReceptionTime < TRANSMIT_DELAY) {
    // Close in time to the previous data, append to buffer
    if (bytesRead < expectedDataSize) {
      buffer[bytesRead++] = Wire.read();
      if (bytesRead == expectedDataSize) {
        newData = true;
      }
    }
  } else {
    // It's been a while, treat this as the start of a new packet
    expectedDataSize = Wire.read();
    if (expectedDataSize > bufferSize) {
      expectedDataSize = bufferSize;
    }
    bytesRead = 0;
    newData = false;
    start();
  }
  lastReceptionTime = now;
}

boolean I2CReader::checksumValid() {
  if (readPosition < bufferSize) {
    byte checksum = 0;
    for (int i = 0; i < readPosition; i++) {
      checksum ^= buffer[i];
    }
    return checksum == getByte();
  } else {
    return false;
  }
}

boolean I2CReader::hasNewData() {
  if (newData) {
    newData = false;
    return true;
  }
  return false;
}

void I2CReader::printBuffer() {
  for (int i = 0; i < bufferSize - 1; i++) {
    Serial.print(buffer[i], DEC);
    Serial.print(F(", "));
  }
  Serial.println(buffer[bufferSize - 1], DEC);
}

