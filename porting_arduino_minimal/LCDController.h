#ifndef LCD_CONTROLLER_H_
#define LCD_CONTROLLER_H_

#include <lvgl.h>

#include "board_drivers.h"

void lv_port_disp_init(const ILI9341_Config_t *config);
void disp_enable_update(void);
void disp_disable_update(void);

#endif
