#ifndef BOARD_DRIVERS_H_
#define BOARD_DRIVERS_H_

#include <stdint.h>

class SPIClass;

struct ILI9341_Config_t {
    SPIClass *spi;
    uint32_t spi_frequency;
    uint32_t cs_pin;
    uint32_t dc_pin;
    uint32_t reset_pin;
    uint16_t hor_res;
    uint16_t ver_res;
};

struct XPT2046_Config_t {
    SPIClass *spi;
    uint32_t spi_frequency;
    uint32_t cs_pin;
    uint32_t irq_pin;
};

struct Board_TouchCalibration_t {
    uint16_t raw_x_min;
    uint16_t raw_x_max;
    uint16_t raw_y_min;
    uint16_t raw_y_max;
};

const ILI9341_Config_t *Board_GetDisplayConfig(void);
const XPT2046_Config_t *Board_GetTouchConfig(void);
uint16_t Board_GetDisplayHorRes(void);
uint16_t Board_GetDisplayVerRes(void);
uint8_t Board_GetDisplayRotation(void);
void Board_GetTouchCalibration(Board_TouchCalibration_t *cal);
void Board_SetTouchCalibration(const Board_TouchCalibration_t *cal);
void Board_ResetTouchCalibration(void);
bool Board_TouchCalibrationIsOverridden(void);

#endif
