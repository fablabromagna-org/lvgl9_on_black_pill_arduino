#include "ui_logic.h"
#include "platform_app.h"
#include "actions.h"
#include "ui.h"
#include "screens.h"
#include "vars.h"

#include <stdint.h>

uint64_t click_cnt = 0;
int32_t blue_button_press_cnt = 0;


// Callback from UI used to assign UI label directly
void action_btn_pressed(lv_event_t *e) {
    (void)e;
    // TODO: Implement action btn_pressed here

        click_cnt++;

        lv_label_set_text_fmt(objects.label_test, "You clicked me\n%llu times", (unsigned long long)click_cnt);
}


//  Callback from UI used to command Nucleo builtin led
void action_switch_led(lv_event_t *e) {
    (void)e;
    platform_led_toggle();
}





/*** Getter and Setter for Native Variables  */


int32_t get_var_blue_button_press_cnt() {
    return blue_button_press_cnt;
}

void set_var_blue_button_press_cnt(int32_t value) {
    blue_button_press_cnt = value;
}
