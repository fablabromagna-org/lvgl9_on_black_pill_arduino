#include "platform_app.h"

#include <Arduino.h>
#include <SPI.h>

#include "board_config.h"

namespace {

bool g_led_state = false;

void write_led(bool on) {
#if BOARD_USER_LED_ACTIVE_LOW
    digitalWrite(BOARD_USER_LED_PIN, on ? LOW : HIGH);
#else
    digitalWrite(BOARD_USER_LED_PIN, on ? HIGH : LOW);
#endif
}

}  // namespace

extern "C" void platform_app_init(void) {
    SPI.begin();
    Serial.begin(BOARD_SERIAL_BAUDRATE);
    delay(200);

    pinMode(BOARD_USER_LED_PIN, OUTPUT);
    write_led(false);

#if BOARD_USER_BUTTON_ENABLED
    pinMode(BOARD_USER_BUTTON_PIN, INPUT_PULLUP);
#endif

    Serial.println("porting_arduino_minimal: serial ready");
}

extern "C" void platform_led_toggle(void) {
    g_led_state = !g_led_state;
    write_led(g_led_state);
}

extern "C" bool platform_button_is_pressed(void) {
#if BOARD_USER_BUTTON_ENABLED
    const int raw = digitalRead(BOARD_USER_BUTTON_PIN);
#if BOARD_USER_BUTTON_ACTIVE_LOW
    return raw == LOW;
#else
    return raw == HIGH;
#endif
#else
    return false;
#endif
}

extern "C" unsigned long platform_millis(void) {
    return millis();
}
