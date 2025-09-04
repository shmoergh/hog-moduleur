#include "toggle.h"

uint32_t Toggle::get_now_() {
    return to_ms_since_boot(get_absolute_time());
}

/* ------------------------ ⬆︎ PRIVATE | PUBLIC ⬇︎  -------------------------- */

Toggle::Toggle(uint8_t pin, uint32_t debounce_time) {
    pin_ = pin;
    debounce_time_ = debounce_time;
}

void Toggle::init_gpio() {
    gpio_init(pin_);
    gpio_set_dir(pin_, GPIO_IN);
    gpio_pull_up(pin_);
}

bool Toggle::is_on() {
    bool btn_read = !gpio_get(pin_);

    if (state_ == toggle_state::OFF_2_ON) {
        if (btn_read) {
            on_ = true;
            previous_debounce_ms_ = get_now_();
        } else {
            if (get_now_() - previous_debounce_ms_ >= debounce_time_ && on_) {
                state_ = toggle_state::ON_2_OFF;
            }
        }
    }

    if (state_ == toggle_state::ON_2_OFF) {
        if (btn_read) {
            off_enabled_ = true;
            previous_debounce_ms_ = get_now_();
        } else {
            if (off_enabled_) {
                if (get_now_() - previous_debounce_ms_ >= debounce_time_) {
                    state_ = toggle_state::OFF_2_ON;
                    off_enabled_ = false;
                    on_ = false;
                }
            }
        }
    }

    return on_;
}