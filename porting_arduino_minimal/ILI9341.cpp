#include "ILI9341.h"

#include <Arduino.h>
#include <SPI.h>

namespace {

const ILI9341_Config_t *g_config = nullptr;
SPISettings g_spi_settings;

inline void cs_low() {
    digitalWrite(g_config->cs_pin, LOW);
}

inline void cs_high() {
    digitalWrite(g_config->cs_pin, HIGH);
}

inline void dc_low() {
    digitalWrite(g_config->dc_pin, LOW);
}

inline void dc_high() {
    digitalWrite(g_config->dc_pin, HIGH);
}

inline void reset_low() {
    digitalWrite(g_config->reset_pin, LOW);
}

inline void reset_high() {
    digitalWrite(g_config->reset_pin, HIGH);
}

}  // namespace

void ILI9341_WriteCommand(uint8_t cmd) {
    dc_low();
    cs_low();
    g_config->spi->transfer(cmd);
    cs_high();
}

void ILI9341_WriteData(uint8_t data) {
    dc_high();
    cs_low();
    g_config->spi->transfer(data);
    cs_high();
}

void ILI9341_WriteData16(uint16_t data) {
    dc_high();
    cs_low();
    g_config->spi->transfer(static_cast<uint8_t>(data >> 8));
    g_config->spi->transfer(static_cast<uint8_t>(data & 0xFF));
    cs_high();
}

void ILI9341_Init(const ILI9341_Config_t *config) {
    g_config = config;
    g_spi_settings = SPISettings(config->spi_frequency, MSBFIRST, SPI_MODE0);
    g_config->spi->begin();
    g_config->spi->beginTransaction(g_spi_settings);

    pinMode(g_config->cs_pin, OUTPUT);
    pinMode(g_config->dc_pin, OUTPUT);
    pinMode(g_config->reset_pin, OUTPUT);

    cs_high();
    dc_high();
    reset_high();

    reset_low();
    delay(20);
    reset_high();
    delay(150);

    ILI9341_WriteCommand(0x01);
    delay(10);
    ILI9341_WriteCommand(0x28);

    ILI9341_WriteCommand(0xCF);
    ILI9341_WriteData(0x00);
    ILI9341_WriteData(0xC1);
    ILI9341_WriteData(0x30);

    ILI9341_WriteCommand(0xED);
    ILI9341_WriteData(0x64);
    ILI9341_WriteData(0x03);
    ILI9341_WriteData(0x12);
    ILI9341_WriteData(0x81);

    ILI9341_WriteCommand(0xE8);
    ILI9341_WriteData(0x85);
    ILI9341_WriteData(0x00);
    ILI9341_WriteData(0x78);

    ILI9341_WriteCommand(0xCB);
    ILI9341_WriteData(0x39);
    ILI9341_WriteData(0x2C);
    ILI9341_WriteData(0x00);
    ILI9341_WriteData(0x34);
    ILI9341_WriteData(0x02);

    ILI9341_WriteCommand(0xF7);
    ILI9341_WriteData(0x20);

    ILI9341_WriteCommand(0xEA);
    ILI9341_WriteData(0x00);
    ILI9341_WriteData(0x00);

    ILI9341_WriteCommand(0xC0);
    ILI9341_WriteData(0x23);

    ILI9341_WriteCommand(0xC1);
    ILI9341_WriteData(0x10);

    ILI9341_WriteCommand(0xC5);
    ILI9341_WriteData(0x3E);
    ILI9341_WriteData(0x28);

    ILI9341_WriteCommand(0xC7);
    ILI9341_WriteData(0x86);

    ILI9341_WriteCommand(0x3A);
    ILI9341_WriteData(0x55);

    ILI9341_WriteCommand(0xB1);
    ILI9341_WriteData(0x00);
    ILI9341_WriteData(0x18);

    ILI9341_WriteCommand(0xB6);
    ILI9341_WriteData(0x08);
    ILI9341_WriteData(0x82);
    ILI9341_WriteData(0x27);

    ILI9341_WriteCommand(0xF2);
    ILI9341_WriteData(0x00);

    ILI9341_WriteCommand(0x26);
    ILI9341_WriteData(0x01);

    ILI9341_WriteCommand(0x11);
    delay(120);
    ILI9341_WriteCommand(0x29);
}

void ILI9341_FillScreen(uint16_t color) {
    const uint16_t w = g_config->hor_res;
    const uint16_t h = g_config->ver_res;
    ILI9341_SetWindow(0, 0, static_cast<uint16_t>(w - 1), static_cast<uint16_t>(h - 1));

    uint8_t line_buf[320 * 2];
    for (uint16_t i = 0; i < w; ++i) {
        line_buf[i * 2] = static_cast<uint8_t>(color >> 8);
        line_buf[i * 2 + 1] = static_cast<uint8_t>(color & 0xFF);
    }

    dc_high();
    cs_low();
    for (uint16_t y = 0; y < h; ++y) {
        g_config->spi->transfer(line_buf, w * 2);
    }
    cs_high();
}

void ILI9341_SetRotation(uint8_t rotation) {
    ILI9341_WriteCommand(0x36);
    switch (rotation) {
        case 0:
            ILI9341_WriteData(0x48);
            break;
        case 1:
            ILI9341_WriteData(0x28);
            break;
        case 2:
            ILI9341_WriteData(0x88);
            break;
        case 3:
        default:
            ILI9341_WriteData(0xE8);
            break;
    }
}

void ILI9341_SetWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    ILI9341_WriteCommand(0x2A);
    ILI9341_WriteData(x0 >> 8);
    ILI9341_WriteData(x0 & 0xFF);
    ILI9341_WriteData(x1 >> 8);
    ILI9341_WriteData(x1 & 0xFF);

    ILI9341_WriteCommand(0x2B);
    ILI9341_WriteData(y0 >> 8);
    ILI9341_WriteData(y0 & 0xFF);
    ILI9341_WriteData(y1 >> 8);
    ILI9341_WriteData(y1 & 0xFF);

    ILI9341_WriteCommand(0x2C);
}

void ILI9341_DrawBitmap(uint16_t w, uint16_t h, uint8_t *s) {
    (void)h;
    dc_high();
    cs_low();
    g_config->spi->transfer(s, w * h * 2);
    cs_high();
}

void ILI9341_DrawBitmapDMA(uint16_t w, uint16_t h, uint8_t *s, lv_display_t *disp) {
    ILI9341_DrawBitmap(w, h, s);
    lv_display_flush_ready(disp);
}

void ILI9341_CancelFlushCallback(void) {}
