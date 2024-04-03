#include "statusled.h"

#include <hardware/gpio.h>

#define LED_PIN 0

void status_led_init()
{
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 0);
}