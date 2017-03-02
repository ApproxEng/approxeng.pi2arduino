#define PER_BYTE_DELAY 10
#define RETRY_DELAY 100

void sendBytes(unsigned address, char *buffer, unsigned length);

void getBytes(unsigned address, char *buffer, unsigned length);