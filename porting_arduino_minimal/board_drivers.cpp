#include "board_drivers.h"

#include <SPI.h>

#include "board_config.h"

namespace {

ILI9341_Config_t g_display_config = {
    .spi = &SPI,
    .spi_frequency = BOARD_TFT_SPI_FREQUENCY,
    .cs_pin = BOARD_TFT_CS_PIN,
    .dc_pin = BOARD_TFT_DC_PIN,
    .reset_pin = BOARD_TFT_RST_PIN,
    .hor_res = BOARD_TFT_HOR_RES,
    .ver_res = BOARD_TFT_VER_RES,
};

XPT2046_Config_t g_touch_config = {
    .spi = &SPI,
    .spi_frequency = BOARD_TOUCH_SPI_FREQUENCY,
    .cs_pin = BOARD_TOUCH_CS_PIN,
    .irq_pin = BOARD_TOUCH_IRQ_PIN,
};

Board_TouchCalibration_t g_touch_defaults = {
    .raw_x_min = BOARD_TOUCH_RAW_X_MIN,
    .raw_x_max = BOARD_TOUCH_RAW_X_MAX,
    .raw_y_min = BOARD_TOUCH_RAW_Y_MIN,
    .raw_y_max = BOARD_TOUCH_RAW_Y_MAX,
};

Board_TouchCalibration_t g_touch_active = g_touch_defaults;
bool g_touch_overridden = false;

}  // namespace

const ILI9341_Config_t *Board_GetDisplayConfig(void) {
    return &g_display_config;
}

const XPT2046_Config_t *Board_GetTouchConfig(void) {
    return &g_touch_config;
}

uint16_t Board_GetDisplayHorRes(void) {
    return g_display_config.hor_res;
}

uint16_t Board_GetDisplayVerRes(void) {
    return g_display_config.ver_res;
}

uint8_t Board_GetDisplayRotation(void) {
    return BOARD_TFT_ROTATION;
}

void Board_GetTouchCalibration(Board_TouchCalibration_t *cal) {
    if (cal == nullptr) {
        return;
    }
    *cal = g_touch_active;
}

void Board_SetTouchCalibration(const Board_TouchCalibration_t *cal) {
    if (cal == nullptr) {
        return;
    }
    if ((cal->raw_x_min >= cal->raw_x_max) || (cal->raw_y_min >= cal->raw_y_max)) {
        return;
    }

    g_touch_active = *cal;
    g_touch_overridden = true;
}

void Board_ResetTouchCalibration(void) {
    g_touch_active = g_touch_defaults;
    g_touch_overridden = false;
}

bool Board_TouchCalibrationIsOverridden(void) {
    return g_touch_overridden;
}
