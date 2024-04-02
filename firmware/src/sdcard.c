#include "sdcard.h"

#include <hardware/gpio.h>
#include <hardware/spi.h>
#include <pico/time.h>

#define CD_PIN   14
#define SCK_PIN  10
#define MOSI_PIN 11
#define MISO_PIN 12
#define CS_PIN   13

#define SD_INIT_TRYS    0x03

uint8_t sdcard_buffer[512] = { 0 };

static bool     mount = false;
static uint8_t  cardtype = 0;
static uint32_t last_sector = 0;

/* Definitions of SD commands */
#define CMD0    (0x40+0)        /* GO_IDLE_STATE            */
#define CMD1    (0x40+1)        /* SEND_OP_COND (MMC)       */
#define ACMD41  (0xC0+41)       /* SEND_OP_COND (SDC)       */
#define CMD8    (0x40+8)        /* SEND_IF_COND             */
#define CMD9    (0x40+9)        /* SEND_CSD                 */
#define CMD16   (0x40+16)       /* SET_BLOCKLEN             */
#define CMD17   (0x40+17)       /* READ_SINGLE_BLOCK        */
#define CMD24   (0x40+24)       /* WRITE_SINGLE_BLOCK       */
#define CMD42   (0x40+42)       /* LOCK_UNLOCK              */
#define CMD55   (0x40+55)       /* APP_CMD                  */
#define CMD58   (0x40+58)       /* READ_OCR                 */
#define CMD59   (0x40+59)       /* CRC_ON_OFF               */

static void sdcard_spi_init()
{
    gpio_set_dir(SCK_PIN, GPIO_IN);
    gpio_set_dir(MOSI_PIN, GPIO_IN);
    gpio_set_dir(CS_PIN, GPIO_IN);

    gpio_set_function(SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(MOSI_PIN, GPIO_FUNC_SPI);
    gpio_set_function(MISO_PIN, GPIO_FUNC_SPI);
    gpio_set_function(CS_PIN, GPIO_FUNC_SPI);

    spi_init(spi1, 400000);
}

static void sdcard_spi_deinit()
{
    gpio_set_function(SCK_PIN, GPIO_FUNC_NULL);
    gpio_set_function(MOSI_PIN, GPIO_FUNC_NULL);
    gpio_set_function(MISO_PIN, GPIO_FUNC_NULL);
    gpio_set_function(CS_PIN, GPIO_FUNC_NULL);

    spi_deinit(spi1);

    gpio_set_dir(SCK_PIN, GPIO_OUT);
    gpio_set_dir(MOSI_PIN, GPIO_OUT);
    gpio_set_dir(CS_PIN, GPIO_OUT);

    gpio_pull_up(CS_PIN);
    gpio_pull_up(MOSI_PIN);
}

static void sdcard_spi_clock()
{
    gpio_put(SCK_PIN, 1);
    sleep_us(200);
    gpio_put(SCK_PIN, 0);
    sleep_us(200);
}

void sdcard_init()
{
    gpio_init(CD_PIN);
}

bool sdcard_present()
{
    return gpio_get(CD_PIN);
}

SDResult sdcard_initialize()
{
    uint8_t n, cmd, ct, ocr[4];
    uint8_t idx;
    uint8_t init_trys;
    ct = 0;

    for(init_trys=0; ((init_trys!=SD_INIT_TRYS)&&(!ct)); init_trys++)
    {
        // Initialize SPI for use with the memory card
        sdcard_spi_deinit();

        // 80 dummy clocks
        for (idx = 0; idx < 10; idx++)
            sdcard_spi_clock();

        sleep_ms(500);
        mount = false;

        // Idle state
        if (sdcard_send_cmd(CMD0, 0) == 1) {
            // SD version 2?
            if (__SD_Send_Cmd(CMD8, 0x1AA) == 1) {
                // Get trailing return value of R7 resp
                for (n = 0; n < 4; n++) ocr[n] = SPI_RW(0xFF);
                // VDD range of 2.7-3.6V is OK?
                if ((ocr[2] == 0x01)&&(ocr[3] == 0xAA))
                {
                    // Wait for leaving idle state (ACMD41 with HCS bit)...
                    SPI_Timer_On(1000);
                    while ((SPI_Timer_Status()==TRUE)&&(__SD_Send_Cmd(ACMD41, 1UL << 30)));
                    SPI_Timer_Off();
                    // CCS in the OCR?
                    if ((SPI_Timer_Status()==TRUE)&&(__SD_Send_Cmd(CMD58, 0) == 0))
                    {
                        for (n = 0; n < 4; n++) ocr[n] = SPI_RW(0xFF);
                        // SD version 2?
                        ct = (ocr[0] & 0x40) ? SDCT_SD2 | SDCT_BLOCK : SDCT_SD2;
                    }
                }
            } else {
                // SD version 1 or MMC?
                if (__SD_Send_Cmd(ACMD41, 0) <= 1)
                {
                    // SD version 1
                    ct = SDCT_SD1;
                    cmd = ACMD41;
                } else {
                    // MMC version 3
                    ct = SDCT_MMC;
                    cmd = CMD1;
                }
                // Wait for leaving idle state
                SPI_Timer_On(250);
                while((SPI_Timer_Status()==TRUE)&&(__SD_Send_Cmd(cmd, 0)));
                SPI_Timer_Off();
                if(SPI_Timer_Status()==FALSE) ct = 0;
                if(__SD_Send_Cmd(CMD59, 0))   ct = 0;   // Deactivate CRC check (default)
                if(__SD_Send_Cmd(CMD16, 512)) ct = 0;   // Set R/W block length to 512 bytes
            }
        }
    }
    if(ct) {
        dev->cardtype = ct;
        dev->mount = TRUE;
        dev->last_sector = __SD_Sectors(dev) - 1;
#ifdef SD_IO_DBG_COUNT
        dev->debug.read = 0;
        dev->debug.write = 0;
#endif
        __SD_Speed_Transfer(HIGH); // High speed transfer
    }
    SPI_Release();
    return (ct ? SD_OK : SD_NOINIT);
#endif
}