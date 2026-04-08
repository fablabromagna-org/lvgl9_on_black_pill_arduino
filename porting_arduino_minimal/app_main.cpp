#include "app_main.h"

#include <Arduino.h>
#include <lvgl.h>

#include "board_config.h"
#include "LCDController.h"
#include "TouchController.h"
#include "platform_app.h"
#include "ui.h"
#include "vars.h"

namespace {

constexpr uint32_t kLoopDelayMs = 5;
constexpr uint32_t kButtonPollPeriodMs = 20;
constexpr uint32_t kSerialHeartbeatPeriodMs = 1000;

uint32_t g_last_button_poll_ms = 0;
bool g_button_latched = false;
uint32_t g_last_serial_heartbeat_ms = 0;

uint32_t lvgl_tick_cb() {
    return millis();
}

void poll_user_button() {
    const uint32_t now = millis();
    if ((now - g_last_button_poll_ms) < kButtonPollPeriodMs) {
        return;
    }

    g_last_button_poll_ms = now;

    const bool pressed = platform_button_is_pressed();
    if (pressed && !g_button_latched) {
        g_button_latched = true;
        set_var_blue_button_press_cnt(get_var_blue_button_press_cnt() + 1);
    } else if (!pressed) {
        g_button_latched = false;
    }
}

void serial_heartbeat() {
#if BOARD_ENABLE_SERIAL_MOUSE
    const uint32_t now = millis();
    if ((now - g_last_serial_heartbeat_ms) < kSerialHeartbeatPeriodMs) {
        return;
    }

    g_last_serial_heartbeat_ms = now;
    Serial.println("heartbeat");
#endif
}

}  // namespace

void app_setup(void) {
    platform_app_init();

    lv_init();
    lv_tick_set_cb(lvgl_tick_cb);

    lv_port_disp_init(Board_GetDisplayConfig());
    TouchController_Init(Board_GetTouchConfig());
    ui_init();

#if BOARD_RUN_TOUCH_CALIBRATION_ON_BOOT
    touch_calibrate();
#endif
}

void app_loop(void) {
    TouchController_Poll();
    lv_timer_handler();
    ui_tick();
    poll_user_button();
    serial_heartbeat();
    delay(kLoopDelayMs);
}
