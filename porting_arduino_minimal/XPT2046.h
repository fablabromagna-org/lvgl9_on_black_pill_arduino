#ifndef XPT2046_H_
#define XPT2046_H_

#include <stdbool.h>
#include <stdint.h>

#include "board_drivers.h"

void XPT2046_Init(const XPT2046_Config_t *config);
bool XPT2046_TouchDetected(void);
bool XPT2046_GetTouch(uint16_t *x, uint16_t *y);

#endif
