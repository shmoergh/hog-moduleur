#include "ui.h"
#include "pico/stdlib.h"

void UI::init() {
    // Set up the LED GPIO
    gpio_init(GP_LED);
    gpio_set_dir(GP_LED, GPIO_OUT);

    // Light up the LED
    gpio_put(GP_LED, 1);

    m_fast_blink = false;
    m_last_blink_time = to_ms_since_boot(get_absolute_time());
    m_led_state = true;
}

void UI::toggle_blink() {
    m_blink_led = !m_blink_led;
}

// Read all values on the UI
void UI::update() {
    if (m_blink_led) {
        // Check if it's time to toggle the LED
        uint32_t current_time = to_ms_since_boot(get_absolute_time());
        uint32_t interval = m_fast_blink ? 50 : 250;

        if (m_fast_blink && m_fast_start_time + m_fast_duration < current_time) {
            m_fast_blink = false;
        }

        if (current_time - m_last_blink_time > interval) {
            m_led_state = !m_led_state;
            gpio_put(GP_LED, m_led_state);

            // Update the last blink time
            m_last_blink_time = current_time;
        }
    }
}

// Blink the LED
void UI::fast_blink(uint16_t duration) {
    m_fast_blink = true;
    m_fast_duration = duration;
    m_fast_start_time = to_ms_since_boot(get_absolute_time());
}