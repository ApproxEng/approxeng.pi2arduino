#include "pigpio.h"
#include "p2a.h"

void sendBytes(unsigned address, char *buffer, unsigned length) {
    unsigned handle = (unsigned)i2cOpen(1, address, 0);
    i2cWriteByte((unsigned)handle, length);
    int i;
    for (i = 0; i < length; i++) {
        i2cWriteByte(handle, (unsigned)buffer[i]);
        gpioDelay(PER_BYTE_DELAY);
    }
    i2cClose(handle);
}

void getBytes(unsigned address, char *buffer, unsigned length) {
    unsigned handle = (unsigned)i2cOpen(1, address, 0);
    i2cReadDevice(handle, buffer, length);
    i2cClose(handle);
}
