#ifndef _UI_H
#define _UI_H

#include <stdio.h>
#include <math.h>
#include <pico/stdio.h>
#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "settings.h"

class UI {
public:
    void init();
    void update();
    void toggle_blink();
    void fast_blink(uint16_t duration);

private:
    bool m_blink_led;
    bool m_fast_blink;
    bool m_led_state;
    uint32_t m_last_blink_time;
    uint16_t m_fast_duration;
    uint32_t m_fast_start_time;
};

#endif