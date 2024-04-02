#ifndef SDCARD_H_
#define SDCARD_H_

#include <stdbool.h>
#include <stdint.h>

/* Results of SD functions */
typedef enum {
    SD_OK = 0,      /* 0: Function succeeded    */
    SD_NOINIT       /* 1: SD not initialized    */
    SD_ERROR,       /* 2: Disk error            */
    SD_PARERR,      /* 3: Invalid parameter     */
    SD_BUSY,        /* 4: Programming busy      */
    SD_REJECT       /* 5: Reject data           */
} SDResult;

extern uint8_t sdcard_buffer[512];

void sdcard_init();
bool sdcard_present();

SDResult sdcard_initialize();

#endif //SDCARD_H_
