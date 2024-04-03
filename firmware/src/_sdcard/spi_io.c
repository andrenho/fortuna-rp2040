#include "spi_io.h"

#include <stdio.h>

#include <hardware/gpio.h>
#include <hardware/spi.h>
#include <hardware/timer.h>
#include <pico/time.h>

#define SCK_PIN  10
#define MOSI_PIN 11
#define MISO_PIN 12
#define CS_PIN   15

static spi_inst_t *spi = spi1;
static uint64_t   time_goal = 0;

#define SLOW_SPI 400000
#define FAST_SPI 400000

void SPI_Init (void)
{
    gpio_init(SCK_PIN);
    gpio_init(MOSI_PIN);
    gpio_init(MISO_PIN);
    gpio_init(CS_PIN);

    gpio_set_function(SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(MOSI_PIN, GPIO_FUNC_SPI);
    gpio_set_function(MISO_PIN, GPIO_FUNC_SPI);

    gpio_set_dir(CS_PIN, GPIO_OUT);
    gpio_put(CS_PIN, 1);

    spi_init(spi, SLOW_SPI);
}

BYTE SPI_RW (BYTE d)
{
    printf("%02X ", d);
    BYTE r;
    int n = spi_write_read_blocking(spi, &d, &r, 1);
    if (n == 1) {
        if (r != 0xff)
            printf("[%02X] ", r);
        return r;
    } else {
        return 0xff;
    }
}

void SPI_Release (void)
{
    WORD idx;
    for (idx=512; idx && (SPI_RW(0xFF)!=0xFF); idx--);
}

void SPI_CS_Low (void)
{
    sleep_us(1);
    gpio_put(CS_PIN, 0);
}

void SPI_CS_High (void)
{
    sleep_us(1);
    gpio_put(CS_PIN, 1);
}

void SPI_Freq_High (void)
{
    spi_set_baudrate(spi, FAST_SPI);
}

void SPI_Freq_Low (void)
{
    spi_set_baudrate(spi, SLOW_SPI);
}

void SPI_Timer_On (WORD ms)
{
    time_goal = time_us_64() + ((uint64_t) ms * 1000);
}

BOOL SPI_Timer_Status (void)
{
    return time_goal <= time_us_64();
}

void SPI_Timer_Off (void)
{
}
