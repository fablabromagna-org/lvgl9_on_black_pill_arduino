#ifndef TOUCH_CONTROLLER_H_
#define TOUCH_CONTROLLER_H_

#include <lvgl.h>

#include "board_drivers.h"

void TouchController_Init(const XPT2046_Config_t *config);
void TouchController_Poll(void);
void touch_calibrate(void);

#endif
