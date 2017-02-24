#include "pigpio.h"
#include "p2a.h"

void sendBytes(int address, char *buffer, int length) {
    gpioInitialise();
    unsigned int handle = i2cOpen(1, address, 0);
    i2cWriteByte(handle, length);
    int i;
    for (i = 0; i < length; i++) {
        i2cWriteByte(handle, buffer[i]);
        gpioDelay(PER_BYTE_DELAY);
    }
    i2cClose(handle);
}

void getBytes(int address, char *buffer, int length) {
    gpioInitialise();
    unsigned int handle = i2cOpen(1, address, 0);
    i2cReadDevice(handle, buffer, length);
    i2cClose(handle);
}