/*
 * I2CHelper.h - Library for handling unreliable I2C communication with a
 * Raspbery Pi. 
 * 
 * Created by Tom Oinn, February 20, 2017
 * 
 * Released under the ASL2
 */

#ifndef I2CHelper_h
#define I2CHelper_h

#include "Arduino.h"

// Delay in milliseconds beyond which we assume we have a new
// packet of data.
#define TRANSMIT_DELAY 10

// Used to respond to I2C requests
class I2CResponder {
  public:
    void start();
    void addFloat(float f);
    void addByte(byte b);
    int write(boolean addChecksum);
    I2CResponder(byte* buffer, int bufferSize);

  private:
    int writePosition;
    byte* buffer;
    int bufferSize;
    boolean dataValid;
    byte checksum;
};

// Used to accumulate individual I2C sent bytes
class I2CReader {
  public:
    void start();
    float getFloat();
    byte getByte();
    boolean checksumValid();
    void receiveByte();
    boolean hasNewData();
    void printBuffer();
    I2CReader(byte* buffer, int bufferSize);

  private:
    int readPosition;
    byte* buffer;
    int bufferSize;
    volatile int expectedDataSize;
    volatile int bytesRead;
    volatile long lastReceptionTime;
    volatile boolean newData;
};

#endif

