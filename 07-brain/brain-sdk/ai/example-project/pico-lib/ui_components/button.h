/**
 * Simple button class with debouncing. It can be used to check if the button
 * was pressed or released.
*/

#ifndef _PICO_LIB_BUTTON_H
#define _PICO_LIB_BUTTON_H

#include <pico/stdlib.h>
#include <hardware/gpio.h>

#define DEFAULT_DEBOUNCE_MS 50
#define DOUBLEPRESS_TRESHOLD_MS 250

class Button
{
private:
    uint8_t pin_;
    uint32_t previous_debounce_ms_;
    uint32_t doublepress_time_ = 0;
    uint32_t debounce_time_;
    bool pressed_ = false;
    bool released_ = false;
    int pressed_count_ = 0;

    uint32_t get_now_();

public:
    Button() { };
    Button (uint8_t pin, uint32_t debounce_time = DEFAULT_DEBOUNCE_MS);

    void init_gpio();
    bool is_pressed();
    bool is_double_pressed();
    bool is_released();
    int get_pressed_count();
};

#endif