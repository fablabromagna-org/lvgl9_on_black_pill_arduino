#ifndef BOARD_CONFIG_H_
#define BOARD_CONFIG_H_

#include <Arduino.h>

#ifndef BOARD_TFT_HOR_RES
#define BOARD_TFT_HOR_RES 320u
#endif

#ifndef BOARD_TFT_VER_RES
#define BOARD_TFT_VER_RES 240u
#endif

#ifndef BOARD_TFT_ROTATION
#define BOARD_TFT_ROTATION 3u
#endif

#ifndef BOARD_TFT_MADCTL
#define BOARD_TFT_MADCTL 0xE8u
#endif

#ifndef BOARD_TFT_SPI_FREQUENCY
#define BOARD_TFT_SPI_FREQUENCY 40000000u
#endif

#ifndef BOARD_TOUCH_SPI_FREQUENCY
#define BOARD_TOUCH_SPI_FREQUENCY 2500000u
#endif

#ifndef BOARD_TOUCH_RAW_X_MIN
#define BOARD_TOUCH_RAW_X_MIN 262u
#endif

#ifndef BOARD_TOUCH_RAW_X_MAX
#define BOARD_TOUCH_RAW_X_MAX 1872u
#endif

#ifndef BOARD_TOUCH_RAW_Y_MIN
#define BOARD_TOUCH_RAW_Y_MIN 230u
#endif

#ifndef BOARD_TOUCH_RAW_Y_MAX
#define BOARD_TOUCH_RAW_Y_MAX 1872u
#endif

#ifndef BOARD_RUN_TOUCH_CALIBRATION_ON_BOOT
#define BOARD_RUN_TOUCH_CALIBRATION_ON_BOOT 0
#endif

#ifndef BOARD_ENABLE_SERIAL_MOUSE
#define BOARD_ENABLE_SERIAL_MOUSE 1
#endif

#ifndef BOARD_SERIAL_BAUDRATE
#define BOARD_SERIAL_BAUDRATE 115200u
#endif

/*
 * Default pin map for Black Pill STM32F411CEU6 using a single shared SPI bus.
 * SPI1: SCK=PA5, MISO=PA6, MOSI=PA7.
 *
 * Suggested wiring:
 * TFT_CS   -> PB0
 * TFT_DC   -> PB1
 * TFT_RST  -> PB10
 * TOUCH_CS -> PB12
 * TOUCH_IRQ-> PB14
 * USER_BTN -> PA0  (optional external button to GND, INPUT_PULLUP)
 * USER_LED -> PC13 (on-board LED, active low on most Black Pill boards)
 */
#ifndef BOARD_TFT_CS_PIN
#define BOARD_TFT_CS_PIN PB0
#endif

#ifndef BOARD_TFT_DC_PIN
#define BOARD_TFT_DC_PIN PB1
#endif

#ifndef BOARD_TFT_RST_PIN
#define BOARD_TFT_RST_PIN PB10
#endif

#ifndef BOARD_TOUCH_CS_PIN
#define BOARD_TOUCH_CS_PIN PB12
#endif

#ifndef BOARD_TOUCH_IRQ_PIN
#define BOARD_TOUCH_IRQ_PIN PB14
#endif

#ifndef BOARD_USER_LED_PIN
#define BOARD_USER_LED_PIN PC13
#endif

#ifndef BOARD_USER_LED_ACTIVE_LOW
#define BOARD_USER_LED_ACTIVE_LOW 1
#endif

#ifndef BOARD_USER_BUTTON_PIN
#define BOARD_USER_BUTTON_PIN PA0
#endif

#ifndef BOARD_USER_BUTTON_ACTIVE_LOW
#define BOARD_USER_BUTTON_ACTIVE_LOW 1
#endif

#ifndef BOARD_USER_BUTTON_ENABLED
#define BOARD_USER_BUTTON_ENABLED 1
#endif

#endif
