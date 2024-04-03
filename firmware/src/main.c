#include "pico/stdlib.h"

#include <stdio.h>

#include "statusled.h"
#include "sd_card.h"

int main()
{
    stdio_usb_init();

    printf("Press any key...\n");
    getchar();

    if (!sd_init_driver()) {
        printf("ERROR!\n");
    } else {
        printf("SUCCESS!\n");
    }

    FATFS fs;
    FRESULT fr = f_mount(&fs, "0:", 1);
    if (fr != FR_OK)
        printf("Mount error\n");
    else
        printf("Mount success\n");

    /*
    status_led_init();
    sdcard_init();

    sdcard_setup();
     */

    /*
    SPI_Init();

    uint8_t i = 0;
    for (;;) {
        printf("Press any key...\n");
        getchar();

        printf("Fortuna-RP2040 initializing...\n");

        if (!sd_present()) {
            printf("SDCard not present!");
            for (;;);
        }

        SD_DEV dev;
        printf("%d\n", SD_Init(&dev));
    }
     */

    for (;;);
}
