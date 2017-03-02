#include "pigpio.h"
#include "p2a.h"
#include <stdio.h>

void sendBytes(unsigned address, char *buffer, unsigned length) {
    unsigned handle = (unsigned) i2cOpen(1, address, 0);
    i2cWriteByte((unsigned) handle, length);
    int i;
    for (i = 0; i < length; i++) {
        int status = -1;
        while (status < 0) {
            status = i2cWriteByte(handle, (unsigned) buffer[i]);
            if (status < 0) {
                switch (status) {
                    case PI_I2C_WRITE_FAILED:
                        fprintf(stderr, "I2C write failed!\n");
                        break;
                    default:
                        fprintf(stderr, "Unknown I2C write failure!\n");
                        break;
                }
                gpioDelay(RETRY_DELAY);
            }
        }
        gpioDelay(PER_BYTE_DELAY);
    }
    i2cClose(handle);
}

void getBytes(unsigned address, char *buffer, unsigned length) {
    unsigned handle = (unsigned) i2cOpen(1, address, 0);
    int bytesRead = i2cReadDevice(handle, buffer, length);
    if (bytesRead < 0) {
        switch (bytesRead) {
            case PI_I2C_READ_FAILED:
                fprintf(stderr, "I2C read failed!\n");
                break;
            case PI_BAD_HANDLE:
                fprintf(stderr, "I2C read - bad handle!\n");
                break;
            case PI_BAD_PARAM:
                fprintf(stderr, "I2C read - bad param!\n");
                break;
            default:
                fprintf(stderr, "I2C read - unknown problem!\n");
                break;
        }
    }
    i2cClose(handle);
}
