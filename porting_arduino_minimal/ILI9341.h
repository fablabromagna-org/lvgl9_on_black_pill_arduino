#ifndef ILI9341_H_
#define ILI9341_H_

#include <stdint.h>
#include <lvgl.h>

#include "board_drivers.h"

void ILI9341_Init(const ILI9341_Config_t *config);
void ILI9341_FillScreen(uint16_t color);
void ILI9341_SetRotation(uint8_t rotation);
void ILI9341_WriteCommand(uint8_t cmd);
void ILI9341_WriteData(uint8_t data);
void ILI9341_WriteData16(uint16_t data);
void ILI9341_CancelFlushCallback(void);
void ILI9341_SetWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void ILI9341_DrawBitmap(uint16_t w, uint16_t h, uint8_t *s);
void ILI9341_DrawBitmapDMA(uint16_t w, uint16_t h, uint8_t *s, lv_display_t *disp);

#endif
