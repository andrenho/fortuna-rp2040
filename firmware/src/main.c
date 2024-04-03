#include "pico/stdlib.h"

#include <stdio.h>

#include "statusled.h"
#include "sdcard.h"

int main()
{
    stdio_usb_init();

    status_led_init();
    sdcard_init();

    printf("Press any key...\n");
    getchar();

    sdcard_setup();

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
