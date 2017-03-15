/*
   I2CHelper.cpp - Library for handling unreliable I2C communication
   with a Raspbery Pi.

   Created by Tom Oinn, February 20, 2017

   Released under the ASL2
*/

#include <Arduino.h>
#include <Wire.h>
#include "I2CHelper.h"

byte _receiveBuffer[RECEIVE_BUFFER_SIZE];
I2CReader I2CHelper::reader(_receiveBuffer, RECEIVE_BUFFER_SIZE);
byte _transmitBuffer[TRANSMIT_BUFFER_SIZE];
I2CResponder I2CHelper::responder(_transmitBuffer, TRANSMIT_BUFFER_SIZE);



volatile int _bytesReceived = 0;
volatile boolean _messageHandled = false;
volatile boolean _bytesProcessing = false;

byte RECEIVE_BUFFER[RECEIVE_BUFFER_SIZE];

void _i2cReceiveCallback(int bytesRead) {
  int bytes = Wire.available();
  _bytesProcessing = true;
  _messageHandled = true;
  _bytesReceived += bytes;
  Wire.readBytes(RECEIVE_BUFFER, bytes);
  int i = 0;
  for (i = 0; i < bytes; i++) {
    I2CHelper::reader.receiveByte(RECEIVE_BUFFER[i]);
  }
  _bytesProcessing = false;
}

void I2CHelper::printReceiveStatus() {
  if (_messageHandled) {
    _messageHandled = false;
    Serial.print(_bytesReceived, DEC);
    Serial.println(F(" bytes received"));
  }
  if (_bytesProcessing) {
    Serial.println("Currently processing interrupt...");
  }
}

void _i2cRequestCallback() {
  if (I2CHelper::user_onRequest) {
    I2CHelper::responder.start();
    I2CHelper::user_onRequest();
  }
}

void I2CHelper::begin(byte address) {
  Wire.onReceive(_i2cReceiveCallback);
  Wire.onRequest(_i2cRequestCallback);
  Wire.begin(address);
#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega8__) || defined(__AVR_ATmega328P__)
  // deactivate internal pull-ups for twi
  // as per note from atmega8 manual pg167
  cbi(PORTC, 4);
  cbi(PORTC, 5);
#elif defined(__AVR_ATmega128__)
  // deactivate internal pull-ups for twi
  // as per note from atmega128 manual pg204
  cbi(PORTD, 0);
  cbi(PORTD, 1);
#endif
  //Serial.println("Waiting for I2C...");
}

void I2CHelper::onRequest(void (*function)(void)) {
  I2CHelper::user_onRequest = function;
}

void (*I2CHelper::user_onRequest)(void);

volatile union floatUnion {
  float f;
  byte fBuff[sizeof(float)];
} fu;

I2CResponder::I2CResponder(byte *b, int bs) {
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
    int bytesSent = -1;
    while (bytesSent != writePosition) {
      bytesSent = Wire.write(buffer, writePosition);
    }
  } else {
    return -1;
  }
}

I2CReader::I2CReader(byte *b, int bs) {
  buffer = b;
  bufferSize = bs;
  readPosition = 0;
  newData = false;
}

void I2CReader::start() {
  bytesRead = 0;
  newData = false;
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

void I2CReader::receiveByte(byte b) {
  long now = millis();
  if (now - lastReceptionTime < TRANSMIT_DELAY && !newData) {
    // Close in time to the previous data, append to buffer
    if (bytesRead < expectedDataSize && bytesRead < bufferSize) {
      buffer[bytesRead++] = b;
      if (bytesRead == expectedDataSize) {
        newData = true;
        readPosition = 0;
      }
    }
  } else {
    // It's been a while, treat this as the start of a new packet
    expectedDataSize = b;
    if (expectedDataSize > bufferSize) {
      expectedDataSize = bufferSize;
    }
    start();
  }
  lastReceptionTime = now;
}

boolean I2CReader::checksumValid() {
  if (bytesRead < bufferSize) {
    byte checksum = 0;
    for (int i = 0; i < bytesRead - 1; i++) {
      checksum ^= buffer[i];
    }
    return checksum == buffer[bytesRead - 1];
  } else {
    return false;
  }
}

boolean I2CReader::hasNewData() {
  if (newData) {
    newData = false;
    _bytesReceived = 0;
    return true;
  }
  return false;
}

void I2CReader::printBuffer() {
  Serial.print(F("bytesRead="));
  Serial.print(bytesRead, DEC);
  Serial.print(F(", readPos="));
  Serial.print(readPosition);
  Serial.print(F(", data="));
  for (int i = 0; i < bufferSize - 1; i++) {
    Serial.print(buffer[i], DEC);
    Serial.print(F(", "));
  }
  Serial.println(buffer[bufferSize - 1], DEC);
}

