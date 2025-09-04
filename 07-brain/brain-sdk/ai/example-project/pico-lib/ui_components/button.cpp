#include "button.h"

uint32_t Button::get_now_() {
    return to_ms_since_boot(get_absolute_time());
}

/* ------------------------ ⬆︎ PRIVATE | PUBLIC ⬇︎  -------------------------- */

Button::Button (uint8_t pin, uint32_t debounce_time) {
    pin_ = pin;
    debounce_time_ = debounce_time;
}

void Button::init_gpio() {
    gpio_init(pin_);
    gpio_set_dir(pin_, GPIO_IN);
    gpio_pull_up(pin_);
}

bool Button::is_pressed() {
    bool btn_read = !gpio_get(pin_);
    uint32_t now = get_now_();

    if (btn_read) {
        pressed_ = true;
        previous_debounce_ms_ = now;
        if (pressed_count_ == 0) pressed_count_ = 1;
        if (pressed_count_ == 1 && now - doublepress_time_ < DOUBLEPRESS_TRESHOLD_MS) {
            pressed_count_ = 2;
        }
    } else {
        if (now - previous_debounce_ms_ > debounce_time_) {
            pressed_ = false;
            if (pressed_count_ == 1 && doublepress_time_ == 0) {
                doublepress_time_ = now;
            }
            if (pressed_count_ == 2 || (pressed_count_ == 1 && now - doublepress_time_ > DOUBLEPRESS_TRESHOLD_MS)) {
                pressed_count_ = 0;
                doublepress_time_ = 0;
            }
        }
    }

    return pressed_;
}

bool Button::is_double_pressed() {
    return pressed_count_ == 2;
}

bool Button::is_released() {
    released_ = false;
    bool btn_read = !gpio_get(pin_);

    if (btn_read) {
        previous_debounce_ms_ = get_now_();
        pressed_ = true;
    } else {
        if ((get_now_() - previous_debounce_ms_ > debounce_time_) && pressed_) {
            released_ = true;
            pressed_ = false;
        }
    }

    return released_;
}

int Button::get_pressed_count() {
    return pressed_count_;
}