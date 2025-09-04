/**
 * Toggle push button
 * 
 * Works like a real toggle button: turns on when pushing them down and turns 
 * off only after they are pushed _and_ released.
*/

#ifndef _PICO_LIB_TOGGLE_H
#define _PICO_LIB_TOGGLE_H
#define DEFAULT_DEBOUNCE_MS 50

#include <pico/stdlib.h>
#include <hardware/gpio.h>

enum toggle_state {OFF_2_ON, ON_2_OFF};

class Toggle {
private:
    uint8_t pin_;
    bool on_ = false;
    bool off_enabled_ = false;
    uint32_t previous_debounce_ms_ = 0;
    uint32_t debounce_time_;
    toggle_state state_ = toggle_state::OFF_2_ON;
    uint32_t get_now_();

public:
    Toggle() {};
    Toggle (uint8_t pin, uint32_t debounce_time = DEFAULT_DEBOUNCE_MS);
    void init_gpio();
    bool is_on();
};

#endif