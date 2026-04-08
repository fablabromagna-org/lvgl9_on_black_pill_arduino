#pragma once

#include <Arduino.h>
#include <SPI.h>

namespace porting {
namespace board {

/*
 * Default pinout for a BlackPill STM32F411CEU6 using SPI1.
 * Adjust these constants to match your actual wiring before flashing.
 */
constexpr uint16_t kDisplayWidth = 320;
constexpr uint16_t kDisplayHeight = 240;
constexpr uint8_t kDisplayRotation = 3;

constexpr uint8_t TFT_SCK = PA5;
constexpr uint8_t TFT_MISO = PA6;
constexpr uint8_t TFT_MOSI = PA7;

constexpr uint8_t TFT_CS = PB0;
constexpr uint8_t TFT_DC = PB1;
constexpr uint8_t TFT_RST = PB10;

constexpr uint8_t TOUCH_CS = PB12;
constexpr uint8_t TOUCH_IRQ = PB13;

constexpr uint16_t TOUCH_RAW_X_MIN = 262;
constexpr uint16_t TOUCH_RAW_X_MAX = 1872;
constexpr uint16_t TOUCH_RAW_Y_MIN = 230;
constexpr uint16_t TOUCH_RAW_Y_MAX = 1872;

inline SPIClass &displaySpi() {
  return SPI;
}

inline SPIClass &touchSpi() {
  return SPI;
}

inline void configurePins() {
  pinMode(TFT_CS, OUTPUT);
  pinMode(TFT_DC, OUTPUT);
  pinMode(TFT_RST, OUTPUT);
  pinMode(TOUCH_CS, OUTPUT);
  pinMode(TOUCH_IRQ, INPUT_PULLUP);

  digitalWrite(TFT_CS, HIGH);
  digitalWrite(TFT_DC, HIGH);
  digitalWrite(TFT_RST, HIGH);
  digitalWrite(TOUCH_CS, HIGH);
}

}  // namespace board
}  // namespace porting
