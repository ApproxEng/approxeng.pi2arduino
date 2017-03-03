#include "pigpio.h"
#include "p2a.h"
#include <stdio.h>

/**
 * Send bytes over I2C, appending a checksum and sending one byte at a
 * time with retry in the event of failure.
 *
 * @param address the I2C device address
 * @param buffer byte data to send
 * @param length length of byte data
 */
void sendBytes(unsigned address, char *buffer, unsigned length) {
    unsigned handle = (unsigned) i2cOpen(1, address, 0);
    // Send the number of bytes of data, +1 for checksum
    i2cWriteByte(handle, length + 1);
    char checksum = 0;
    int i;
    for (i = 0; i < length; i++) {
        checksum ^= buffer[i];
        sendByteWithRetry(handle, (unsigned) buffer[i]);
        gpioDelay(PER_BYTE_DELAY);
    }
    sendByteWithRetry(handle, (unsigned) checksum);
    i2cClose(handle);
}

/**
 * Send a single byte, with retry in the event of failure, using a
 * previously opened I2C device handle.
 *
 * @param handle device handle returned from pigpio.i2cOpen(..)
 * @param byte the byte to send
 */
void sendByteWithRetry(unsigned handle, unsigned byte) {
    int status = -1;
    int retriesRemaining = 4;
    while (status < 0 && retriesRemaining > 0) {
        status = i2cWriteByte(handle, byte);
        if (status < 0) {
            retriesRemaining--;
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
}

/**
 * Get bytes from an I2C device, reading them into the supplied buffer.
 *
 * @param address I2C address from which to request data
 * @param buffer byte buffer of an appropriate size
 * @param length number of bytes to read into the buffer
 */
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
