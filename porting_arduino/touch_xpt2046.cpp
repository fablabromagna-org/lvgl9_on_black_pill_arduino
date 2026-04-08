#include "touch_xpt2046.h"

#include <Arduino.h>
#include <SPI.h>
#include <lvgl.h>

#include "board_blackpill_f411.h"

namespace porting {
namespace touch {
namespace {

SPISettings kTouchSpiSettings(2500000, MSBFIRST, SPI_MODE0);
lv_indev_t *touch_indev = nullptr;

bool touch_pressed = false;
uint16_t touch_x = 0;
uint16_t touch_y = 0;

uint16_t spiXfer(uint8_t cmd) {
  uint8_t tx[3] = {cmd, 0x00, 0x00};
  uint8_t rx[3] = {0x00, 0x00, 0x00};

  digitalWrite(porting::board::TOUCH_CS, LOW);
  porting::board::touchSpi().transfer(tx, rx, sizeof(tx));
  digitalWrite(porting::board::TOUCH_CS, HIGH);

  return static_cast<uint16_t>(((rx[1] << 8) | rx[2]) >> 4);
}

bool getRawTouch(uint16_t *x, uint16_t *y) {
  if (digitalRead(porting::board::TOUCH_IRQ) != LOW) {
    return false;
  }

  constexpr uint8_t kCmdX = 0xD0;
  constexpr uint8_t kCmdY = 0x90;

  spiXfer(kCmdX);
  uint16_t x1 = spiXfer(kCmdX);
  uint16_t x2 = spiXfer(kCmdX);
  spiXfer(kCmdY);
  uint16_t y1 = spiXfer(kCmdY);
  uint16_t y2 = spiXfer(kCmdY);

  *x = static_cast<uint16_t>((x1 + x2) >> 1);
  *y = static_cast<uint16_t>((y1 + y2) >> 1);
  return true;
}

lv_coord_t mapX(uint16_t raw) {
  if (raw <= porting::board::TOUCH_RAW_Y_MIN) {
    raw = porting::board::TOUCH_RAW_Y_MIN;
  }
  if (raw >= porting::board::TOUCH_RAW_Y_MAX) {
    raw = porting::board::TOUCH_RAW_Y_MAX;
  }

  return static_cast<lv_coord_t>(
      static_cast<uint32_t>(raw - porting::board::TOUCH_RAW_Y_MIN) *
      (porting::board::kDisplayWidth - 1) /
      (porting::board::TOUCH_RAW_Y_MAX - porting::board::TOUCH_RAW_Y_MIN));
}

lv_coord_t mapY(uint16_t raw) {
  if (raw <= porting::board::TOUCH_RAW_X_MIN) {
    raw = porting::board::TOUCH_RAW_X_MIN;
  }
  if (raw >= porting::board::TOUCH_RAW_X_MAX) {
    raw = porting::board::TOUCH_RAW_X_MAX;
  }

  return static_cast<lv_coord_t>(
      static_cast<uint32_t>(raw - porting::board::TOUCH_RAW_X_MIN) *
      (porting::board::kDisplayHeight - 1) /
      (porting::board::TOUCH_RAW_X_MAX - porting::board::TOUCH_RAW_X_MIN));
}

void read_cb(lv_indev_t *indev, lv_indev_data_t *data) {
  (void)indev;
  data->state = touch_pressed ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
  data->point.x = static_cast<lv_coord_t>(touch_x);
  data->point.y = static_cast<lv_coord_t>(touch_y);
}

}  // namespace

void begin() {
  porting::board::touchSpi().begin();
  porting::board::touchSpi().beginTransaction(kTouchSpiSettings);

  touch_indev = lv_indev_create();
  lv_indev_set_type(touch_indev, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(touch_indev, read_cb);
}

void poll() {
  uint16_t raw_x = 0;
  uint16_t raw_y = 0;

  if (!getRawTouch(&raw_x, &raw_y)) {
    touch_pressed = false;
    return;
  }

  touch_pressed = true;
  touch_x = static_cast<uint16_t>(mapX(raw_y));
  touch_y = static_cast<uint16_t>(mapY(raw_x));
}

}  // namespace touch
}  // namespace porting
