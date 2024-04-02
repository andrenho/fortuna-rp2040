#include "pico/stdlib.h"

#include <stdio.h>

#include "sdcard.h"

int main()
{
    stdio_usb_init();

    printf("Press any key...\n");
    getchar();

    printf("Fortuna-RP2040 initializing...\n");

    sdcard_init();
    if (!sdcard_present()) {
        printf("SDCard not present!");
        for (;;);
    }

    for (;;);
}
