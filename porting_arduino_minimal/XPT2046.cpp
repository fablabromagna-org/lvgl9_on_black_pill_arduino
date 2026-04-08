#include "XPT2046.h"

#include <Arduino.h>
#include <SPI.h>

namespace {

constexpr uint8_t kCmdX = 0xD0;
constexpr uint8_t kCmdY = 0x90;

const XPT2046_Config_t *g_config = nullptr;
SPISettings g_spi_settings;

uint16_t spi_xfer(uint8_t cmd) {
    uint8_t tx[3] = {cmd, 0, 0};
    uint8_t rx[3] = {0, 0, 0};

    g_config->spi->beginTransaction(g_spi_settings);
    digitalWrite(g_config->cs_pin, LOW);
    for (size_t i = 0; i < 3; ++i) {
        rx[i] = g_config->spi->transfer(tx[i]);
    }
    digitalWrite(g_config->cs_pin, HIGH);
    g_config->spi->endTransaction();

    return static_cast<uint16_t>(((static_cast<uint16_t>(rx[1]) << 8) | rx[2]) >> 4);
}

}  // namespace

void XPT2046_Init(const XPT2046_Config_t *config) {
    g_config = config;
    g_spi_settings = SPISettings(config->spi_frequency, MSBFIRST, SPI_MODE0);

    pinMode(g_config->cs_pin, OUTPUT);
    digitalWrite(g_config->cs_pin, HIGH);

    pinMode(g_config->irq_pin, INPUT_PULLUP);
}

bool XPT2046_TouchDetected(void) {
    return digitalRead(g_config->irq_pin) == LOW;
}

bool XPT2046_GetTouch(uint16_t *x, uint16_t *y) {
    if (!XPT2046_TouchDetected()) {
        return false;
    }

    spi_xfer(kCmdX);
    const uint16_t x1 = spi_xfer(kCmdX);
    const uint16_t x2 = spi_xfer(kCmdX);
    spi_xfer(kCmdY);
    const uint16_t y1 = spi_xfer(kCmdY);
    const uint16_t y2 = spi_xfer(kCmdY);

    *x = static_cast<uint16_t>((x1 + x2) >> 1);
    *y = static_cast<uint16_t>((y1 + y2) >> 1);
    return true;
}
