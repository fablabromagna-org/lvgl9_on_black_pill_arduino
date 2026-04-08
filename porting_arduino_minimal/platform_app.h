#ifndef PLATFORM_APP_H_
#define PLATFORM_APP_H_

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void platform_app_init(void);
void platform_led_toggle(void);
bool platform_button_is_pressed(void);
unsigned long platform_millis(void);

#ifdef __cplusplus
}
#endif

#endif
