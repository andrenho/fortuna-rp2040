#include "sdcard.h"

#include <stdio.h>

#include <hardware/gpio.h>
#include <hardware/spi.h>
#include <pico/time.h>

#define DEBUG_SPI

#define SCK_PIN  2
#define MOSI_PIN 3
#define MISO_PIN 4
#define CS_PIN   5
#define CD_PIN   6
#define SPI_     spi0

#define SD_TIMEOUT 100

#define R1_IDLE_STATE (1 << 0)

typedef enum { M_SPI, M_NON_SPI } SpiMode;
static SpiMode spi_mode;

static void init_spi(SpiMode mode)
{
    if (mode == M_NON_SPI) {
        spi_deinit(SPI_);

        gpio_set_function(SCK_PIN, GPIO_FUNC_NULL);
        gpio_set_function(MOSI_PIN, GPIO_FUNC_NULL);
        gpio_set_function(MISO_PIN, GPIO_FUNC_NULL);
        gpio_set_function(CS_PIN, GPIO_FUNC_NULL);

        gpio_set_dir(CS_PIN, GPIO_OUT);
        gpio_put(CS_PIN, 1);

        gpio_set_dir(SCK_PIN, GPIO_OUT);
        gpio_put(SCK_PIN, 0);

        gpio_set_dir(MOSI_PIN, GPIO_OUT);
        gpio_put(MISO_PIN, 0);

        gpio_set_dir(MISO_PIN, GPIO_IN);

    } else if (mode == M_SPI) {
        spi_init(SPI_, 400000);

        spi_set_format(SPI_, 8, 0, 0, SPI_MSB_FIRST);

        gpio_set_function(SCK_PIN, GPIO_FUNC_SPI);
        gpio_set_function(MOSI_PIN, GPIO_FUNC_SPI);
        gpio_set_function(MISO_PIN, GPIO_FUNC_SPI);
        gpio_set_function(CS_PIN, GPIO_FUNC_SPI);
    }
    spi_mode = mode;
}

static bool cmd(uint8_t command, uint32_t arg, uint8_t crc)
{
    uint8_t bufw[6] = {
            0x40 | command,
            arg >> 24,
            arg >> 16,
            arg >> 8,
            arg,
            crc
    };

    spi_write_blocking(SPI_, bufw, 6);
#ifdef DEBUG_SPI
    printf("[%02X %02X %02X %02X %02X %02X] ", bufw[0], bufw[1], bufw[2], bufw[3], bufw[4], bufw[5]);
#endif

    uint8_t r;
    for (size_t i = 0; i < SD_TIMEOUT; ++i) {
        spi_read_blocking(SPI_, 0xff, &r, 1);
#ifdef DEBUG_SPI
        printf("%02X ", r);
#endif
    }

    return S_OK;
}

static void first_cycles()
{
    init_spi(M_NON_SPI);

    gpio_put(CS_PIN, 1);
    gpio_put(MOSI_PIN, 1);
    for (size_t i = 0; i < 200; ++i) {
        gpio_put(SCK_PIN, 1);
        sleep_us(10);
        gpio_put(SCK_PIN, 0);
        sleep_us(10);
    }
}

void sdcard_init()
{
    gpio_init(SCK_PIN);
    gpio_init(MOSI_PIN);
    gpio_init(MISO_PIN);
    gpio_init(CS_PIN);
    gpio_init(CD_PIN);

    gpio_set_dir(CD_PIN, GPIO_IN);
}

SDCardStatus sdcard_setup()
{
    first_cycles();

    init_spi(M_SPI);
    for (size_t i = 0; i < 5; ++i) {
        if (cmd(0, 0, 0x95) == R1_IDLE_STATE)
            goto init_passed;
    }

init_passed:

}