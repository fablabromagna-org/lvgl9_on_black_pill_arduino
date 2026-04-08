#include "app_ui.h"

#include <lvgl.h>

namespace porting {
namespace app {
namespace {

lv_obj_t *time_label = nullptr;
lv_obj_t *date_label = nullptr;
uint32_t press_counter = 0;
uint32_t last_label_update_ms = 0;

void btn_event_cb(lv_event_t *e) {
  if (lv_event_get_code(e) != LV_EVENT_CLICKED) {
    return;
  }

  press_counter++;
  lv_label_set_text_fmt(date_label, "Clicked %lu", static_cast<unsigned long>(press_counter));
}

}  // namespace

void create_ui() {
  lv_obj_t *scr = lv_scr_act();
  lv_obj_set_style_bg_color(scr, lv_color_hex(0xffffff), LV_PART_MAIN);

  static lv_style_t style_label;
  static lv_style_t style_btn;
  static lv_style_t style_btn_pressed;
  static bool styles_initialized = false;

  if (!styles_initialized) {
    styles_initialized = true;

    lv_style_init(&style_label);
    lv_style_set_text_font(&style_label, &lv_font_montserrat_24);

    lv_style_init(&style_btn);
    lv_style_set_bg_color(&style_btn, lv_color_hex(0x00ff00));
    lv_style_set_radius(&style_btn, 10);
    lv_style_set_border_width(&style_btn, 2);
    lv_style_set_border_color(&style_btn, lv_color_hex(0x1f618d));
    lv_style_set_pad_all(&style_btn, 10);

    lv_style_init(&style_btn_pressed);
    lv_style_set_bg_color(&style_btn_pressed, lv_color_hex(0x1f618d));
    lv_style_set_transform_width(&style_btn_pressed, -2);
    lv_style_set_transform_height(&style_btn_pressed, -2);
  }

  lv_obj_t *label = lv_label_create(scr);
  lv_label_set_text(label, "FabLab Romagna");
  lv_obj_align(label, LV_ALIGN_CENTER, 0, -60);

  date_label = lv_label_create(scr);
  lv_label_set_text(date_label, "LVGL on Black Pill");
  lv_obj_align(date_label, LV_ALIGN_CENTER, 0, -20);

  time_label = lv_label_create(scr);
  lv_label_set_text(time_label, "0");
  lv_obj_align(time_label, LV_ALIGN_CENTER, 0, 20);
  lv_obj_add_style(time_label, &style_label, 0);

  lv_obj_t *btn = lv_btn_create(scr);
  lv_obj_align(btn, LV_ALIGN_CENTER, 0, 80);
  lv_obj_add_style(btn, &style_btn, 0);
  lv_obj_add_style(btn, &style_btn_pressed, LV_STATE_PRESSED);
  lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_ALL, nullptr);

  lv_obj_t *btn_label = lv_label_create(btn);
  lv_label_set_text(btn_label, "Click");
  lv_obj_center(btn_label);
  lv_obj_add_style(btn_label, &style_label, 0);
}

void update_runtime(uint32_t now_ms) {
  if (time_label == nullptr) {
    return;
  }

  if (now_ms - last_label_update_ms < 100) {
    return;
  }

  last_label_update_ms = now_ms;
  lv_label_set_text_fmt(time_label, "%lu", static_cast<unsigned long>(now_ms));
}

}  // namespace app
}  // namespace porting
