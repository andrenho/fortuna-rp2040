#ifndef SDCARD_H_
#define SDCARD_H_

typedef enum {
    S_OK, S_NO_CARD, S_VERSION_ERROR, S_NO_RESPONSE, S_FORMAT_NOT_SUPPORTED, S_NO_512_BLOCK_SIZE, S_TIMEOUT
} SDCardStatus;

void sdcard_init();

SDCardStatus sdcard_setup();

#endif //SDCARD_H_
