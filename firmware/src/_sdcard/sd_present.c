#include "sd_present.h"

#include <hardware/gpio.h>

#define CD_PIN 14

static bool initialized = false;

bool sd_present()
{
    if (!initialized) {
        gpio_init(CD_PIN);
        gpio_set_dir(CD_PIN, GPIO_IN);
        initialized = true;
    }

    return gpio_get(CD_PIN);
}