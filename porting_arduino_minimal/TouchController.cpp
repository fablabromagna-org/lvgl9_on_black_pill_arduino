#include "TouchController.h"

#include <Arduino.h>
#include <stdlib.h>
#include <string.h>

#include "board_config.h"
#include "XPT2046.h"

namespace {

volatile bool g_touch_pressed = false;
volatile uint16_t g_touch_x = 0;
volatile uint16_t g_touch_y = 0;
Board_TouchCalibration_t g_touch_cal = {};
lv_obj_t *g_cursor = nullptr;

#if BOARD_ENABLE_SERIAL_MOUSE
constexpr size_t kSerialMouseBufSize = 64;
char g_serial_mouse_buf[kSerialMouseBufSize];
size_t g_serial_mouse_len = 0;
bool g_serial_mouse_active = false;
uint32_t g_serial_mouse_last_update_ms = 0;
#endif

lv_coord_t map_x(uint16_t raw) {
    lv_coord_t w = lv_disp_get_hor_res(nullptr);
    if (raw <= g_touch_cal.raw_y_min) raw = g_touch_cal.raw_y_min;
    if (raw >= g_touch_cal.raw_y_max) raw = g_touch_cal.raw_y_max;
    return static_cast<lv_coord_t>(
        (static_cast<uint32_t>(raw - g_touch_cal.raw_y_min) * (w - 1)) /
        (g_touch_cal.raw_y_max - g_touch_cal.raw_y_min));
}

lv_coord_t map_y(uint16_t raw) {
    lv_coord_t h = lv_disp_get_ver_res(nullptr);
    if (raw <= g_touch_cal.raw_x_min) raw = g_touch_cal.raw_x_min;
    if (raw >= g_touch_cal.raw_x_max) raw = g_touch_cal.raw_x_max;
    return static_cast<lv_coord_t>(
        (static_cast<uint32_t>(raw - g_touch_cal.raw_x_min) * (h - 1)) /
        (g_touch_cal.raw_x_max - g_touch_cal.raw_x_min));
}

void touchpad_read(lv_indev_t *indev, lv_indev_data_t *data) {
    (void)indev;
    data->state = g_touch_pressed ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
    data->point.x = static_cast<lv_coord_t>(g_touch_x);
    data->point.y = static_cast<lv_coord_t>(g_touch_y);
}

void update_cursor_position() {
    if (g_cursor == nullptr) {
        return;
    }

    lv_obj_set_pos(
        g_cursor,
        static_cast<lv_coord_t>(g_touch_x) - 6,
        static_cast<lv_coord_t>(g_touch_y) - 6);

    if (g_touch_pressed) {
        lv_obj_set_style_bg_color(g_cursor, lv_color_hex(0xFF0000), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(g_cursor, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    } else {
        lv_obj_set_style_bg_color(g_cursor, lv_color_hex(0xFFFF00), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(g_cursor, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    }
}

#if BOARD_ENABLE_SERIAL_MOUSE
void apply_serial_mouse_packet(const char *line) {
    if (line[0] != 'M') {
        return;
    }

    char *end_ptr = nullptr;
    long x = strtol(line + 1, &end_ptr, 10);
    if (end_ptr == line + 1) {
        return;
    }

    long y = strtol(end_ptr, &end_ptr, 10);
    if (end_ptr == nullptr) {
        return;
    }

    long pressed = strtol(end_ptr, &end_ptr, 10);

    const lv_coord_t w = lv_disp_get_hor_res(nullptr);
    const lv_coord_t h = lv_disp_get_ver_res(nullptr);

    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x >= w) x = w - 1;
    if (y >= h) y = h - 1;

    g_touch_x = static_cast<uint16_t>(x);
    g_touch_y = static_cast<uint16_t>(y);
    g_touch_pressed = (pressed != 0);
    update_cursor_position();
    g_serial_mouse_active = true;
    g_serial_mouse_last_update_ms = millis();
    Serial.print("mouse ");
    Serial.print(g_touch_x);
    Serial.print(' ');
    Serial.print(g_touch_y);
    Serial.print(' ');
    Serial.println(g_touch_pressed ? 1 : 0);
}

void poll_serial_mouse() {
    while (Serial.available() > 0) {
        const char ch = static_cast<char>(Serial.read());
        if (ch == '\r') {
            continue;
        }

        if (ch == '\n') {
            g_serial_mouse_buf[g_serial_mouse_len] = '\0';
            apply_serial_mouse_packet(g_serial_mouse_buf);
            g_serial_mouse_len = 0;
            continue;
        }

        if (g_serial_mouse_len < (kSerialMouseBufSize - 1)) {
            g_serial_mouse_buf[g_serial_mouse_len++] = ch;
        } else {
            g_serial_mouse_len = 0;
        }
    }

    if (g_serial_mouse_active && (millis() - g_serial_mouse_last_update_ms) > 1000U) {
        g_serial_mouse_active = false;
        g_touch_pressed = false;
        update_cursor_position();
    }
}
#endif

}  // namespace

void TouchController_Init(const XPT2046_Config_t *config) {
    XPT2046_Init(config);
    Board_GetTouchCalibration(&g_touch_cal);
    g_touch_pressed = false;

    g_cursor = lv_obj_create(lv_layer_top());
    lv_obj_remove_style_all(g_cursor);
    lv_obj_set_size(g_cursor, 12, 12);
    lv_obj_set_style_radius(g_cursor, LV_RADIUS_CIRCLE, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(g_cursor, LV_OPA_COVER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(g_cursor, lv_color_hex(0xFFFF00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(g_cursor, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(g_cursor, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_clear_flag(g_cursor, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(g_cursor, LV_OBJ_FLAG_IGNORE_LAYOUT);
    update_cursor_position();

    lv_indev_t *indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, touchpad_read);
}

void TouchController_Poll(void) {
#if BOARD_ENABLE_SERIAL_MOUSE
    poll_serial_mouse();
    if (g_serial_mouse_active) {
        return;
    }
#endif

    uint16_t xr = 0;
    uint16_t yr = 0;

    if (XPT2046_GetTouch(&xr, &yr)) {
        g_touch_pressed = true;
        g_touch_x = static_cast<uint16_t>(map_x(yr));
        g_touch_y = static_cast<uint16_t>(map_y(xr));
        update_cursor_position();
    } else {
        g_touch_pressed = false;
        update_cursor_position();
    }
}

void touch_calibrate(void) {
    // Keep the same calibration workflow as the Cube project out of the first port.
    // Override BOARD_TOUCH_RAW_* or Board_SetTouchCalibration() if your panel needs tuning.
}
