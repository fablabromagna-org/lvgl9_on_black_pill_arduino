#include "display_ili9341_lvgl.h"

#include <Arduino.h>
#include <SPI.h>
#include <lvgl.h>

#include "board_blackpill_f411.h"

namespace porting {
namespace display {
namespace {

SPISettings kDisplaySpiSettings(40000000, MSBFIRST, SPI_MODE0);
lv_display_t *display = nullptr;

/*
 * Two 10-line buffers keep RAM usage reasonable on STM32F411 while still
 * allowing partial rendering with LVGL 9.
 */
uint8_t draw_buf_1[porting::board::kDisplayWidth * 10 * 2];
uint8_t draw_buf_2[porting::board::kDisplayWidth * 10 * 2];

void writeCommand(uint8_t cmd) {
  digitalWrite(porting::board::TFT_DC, LOW);
  digitalWrite(porting::board::TFT_CS, LOW);
  porting::board::displaySpi().transfer(cmd);
  digitalWrite(porting::board::TFT_CS, HIGH);
}

void writeData8(uint8_t data) {
  digitalWrite(porting::board::TFT_DC, HIGH);
  digitalWrite(porting::board::TFT_CS, LOW);
  porting::board::displaySpi().transfer(data);
  digitalWrite(porting::board::TFT_CS, HIGH);
}

void writeData16(uint16_t data) {
  digitalWrite(porting::board::TFT_DC, HIGH);
  digitalWrite(porting::board::TFT_CS, LOW);
  porting::board::displaySpi().transfer(static_cast<uint8_t>(data >> 8));
  porting::board::displaySpi().transfer(static_cast<uint8_t>(data & 0xff));
  digitalWrite(porting::board::TFT_CS, HIGH);
}

void setAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
  writeCommand(0x2A);
  writeData16(x0);
  writeData16(x1);

  writeCommand(0x2B);
  writeData16(y0);
  writeData16(y1);

  writeCommand(0x2C);
}

void hardwareReset() {
  digitalWrite(porting::board::TFT_RST, LOW);
  delay(20);
  digitalWrite(porting::board::TFT_RST, HIGH);
  delay(150);
}

void initPanel() {
  hardwareReset();

  writeCommand(0x01);
  delay(10);
  writeCommand(0x28);

  writeCommand(0xCF);
  writeData8(0x00);
  writeData8(0xC1);
  writeData8(0x30);

  writeCommand(0xED);
  writeData8(0x64);
  writeData8(0x03);
  writeData8(0x12);
  writeData8(0x81);

  writeCommand(0xE8);
  writeData8(0x85);
  writeData8(0x00);
  writeData8(0x78);

  writeCommand(0xCB);
  writeData8(0x39);
  writeData8(0x2C);
  writeData8(0x00);
  writeData8(0x34);
  writeData8(0x02);

  writeCommand(0xF7);
  writeData8(0x20);

  writeCommand(0xEA);
  writeData8(0x00);
  writeData8(0x00);

  writeCommand(0xC0);
  writeData8(0x23);

  writeCommand(0xC1);
  writeData8(0x10);

  writeCommand(0xC5);
  writeData8(0x3E);
  writeData8(0x28);

  writeCommand(0xC7);
  writeData8(0x86);

  writeCommand(0x3A);
  writeData8(0x55);

  writeCommand(0xB1);
  writeData8(0x00);
  writeData8(0x18);

  writeCommand(0xB6);
  writeData8(0x08);
  writeData8(0x82);
  writeData8(0x27);

  writeCommand(0xF2);
  writeData8(0x00);

  writeCommand(0x26);
  writeData8(0x01);

  writeCommand(0x36);
  switch (porting::board::kDisplayRotation) {
    case 0:
      writeData8(0x48);
      break;
    case 1:
      writeData8(0x28);
      break;
    case 2:
      writeData8(0x88);
      break;
    default:
      writeData8(0xE8);
      break;
  }

  writeCommand(0x11);
  delay(120);
  writeCommand(0x29);
}

void flush_cb(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map) {
  const uint32_t width = static_cast<uint32_t>(area->x2 - area->x1 + 1);
  const uint32_t height = static_cast<uint32_t>(area->y2 - area->y1 + 1);
  const uint32_t byte_count = width * height * 2;

  setAddressWindow(area->x1, area->y1, area->x2, area->y2);

  digitalWrite(porting::board::TFT_DC, HIGH);
  digitalWrite(porting::board::TFT_CS, LOW);
  porting::board::displaySpi().transfer(px_map, byte_count);
  digitalWrite(porting::board::TFT_CS, HIGH);

  lv_display_flush_ready(disp);
}

}  // namespace

void begin() {
  porting::board::displaySpi().begin();
  porting::board::displaySpi().beginTransaction(kDisplaySpiSettings);
  initPanel();

  display = lv_display_create(porting::board::kDisplayWidth, porting::board::kDisplayHeight);
  lv_display_set_flush_cb(display, flush_cb);
  lv_display_set_buffers(
      display,
      draw_buf_1,
      draw_buf_2,
      sizeof(draw_buf_1),
      LV_DISPLAY_RENDER_MODE_PARTIAL);
}

}  // namespace display
}  // namespace porting
