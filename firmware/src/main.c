#include "pico/stdlib.h"

#include <stdio.h>

int main()
{
    stdio_usb_init();

    getchar();

    for (;;);
}
