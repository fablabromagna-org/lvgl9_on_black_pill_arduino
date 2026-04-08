#include <Arduino.h>
#include <SPI.h>
#include <lvgl.h>

#include "app_ui.h"
#include "board_blackpill_f411.h"
#include "display_ili9341_lvgl.h"
#include "touch_xpt2046.h"

namespace {

constexpr uint32_t kLvglTickPeriodMs = 5;
constexpr uint32_t kLvglHandlerPeriodMs = 5;

uint32_t last_tick_ms = 0;
uint32_t last_handler_ms = 0;

void log_line(const char *msg) {
  Serial.println(msg);
}

}  // namespace

void setup() {
  Serial.begin(115200);
  delay(100);

  log_line("porting_arduino: boot");

  lv_init();

  porting::board::configurePins();
  porting::display::begin();
  porting::touch::begin();

  porting::app::create_ui();

  last_tick_ms = millis();
  last_handler_ms = last_tick_ms;

  log_line("porting_arduino: init complete");
}

void loop() {
  const uint32_t now = millis();

  if (now - last_tick_ms >= kLvglTickPeriodMs) {
    const uint32_t elapsed = now - last_tick_ms;
    last_tick_ms = now;
    lv_tick_inc(elapsed);
    porting::app::update_runtime(now);
  }

  if (now - last_handler_ms >= kLvglHandlerPeriodMs) {
    last_handler_ms = now;
    porting::touch::poll();
    lv_timer_handler();
  }

  delay(1);
}
