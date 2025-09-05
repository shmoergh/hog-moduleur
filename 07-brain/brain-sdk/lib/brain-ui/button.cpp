#include "brain-ui/button.h"

#include <stdio.h>

namespace brain {
namespace ui {

Button::Button(uint gpio_pin, uint32_t debounce_ms, uint32_t long_press_ms) :
	gpio_pin_(gpio_pin),
	is_pressed_(false),
	debounce_ms_(debounce_ms),
	long_press_ms_(long_press_ms),
	long_press_triggered_(false) {}

void Button::Init(bool pull_up) {
	gpio_init(gpio_pin_);
	gpio_set_dir(gpio_pin_, GPIO_IN);
	if (pull_up) {
		gpio_pull_up(gpio_pin_);
	} else {
		gpio_pull_down(gpio_pin_);
	}
	is_pressed_ = false;
	last_press_time_ = get_absolute_time();
	last_release_time_ = get_absolute_time();
	last_tap_time_ = get_absolute_time();
	long_press_triggered_ = false;
}

void Button::Update() {
	bool current_state = gpio_get(gpio_pin_);
	absolute_time_t now = get_absolute_time();

	// Debounce logic
	static bool last_state = false;
	static absolute_time_t last_change_time = 0;
	if (current_state != last_state) {
		if (absolute_time_diff_us(last_change_time, now) / 1000 >= debounce_ms_) {
			last_state = current_state;
			last_change_time = now;
			if (!current_state) {
				// Pressed (active low)
				last_press_time_ = now;
				is_pressed_ = true;
				long_press_triggered_ = false;
				if (on_press_) on_press_();
				last_tap_time_ = now;
			} else {
				// Released (inactive high)
				last_release_time_ = now;
				is_pressed_ = false;
				if (on_release_) on_release_();
				// Single tap detection
				if (last_tap_time_ > 0 &&
					absolute_time_diff_us(last_tap_time_, now) / 1000 > 50) {  // 50ms threshold
					if (on_single_tap_) on_single_tap_();
					last_tap_time_ = 0;
				}
			}
		}
	}

	// Long press detection
	if (is_pressed_ && !long_press_triggered_ &&
		absolute_time_diff_us(last_press_time_, now) / 1000 >= long_press_ms_) {
		long_press_triggered_ = true;
		if (on_long_press_) on_long_press_();
	}
}

void Button::SetOnPress(std::function<void()> callback) {
	on_press_ = callback;
}
void Button::SetOnRelease(std::function<void()> callback) {
	on_release_ = callback;
}
void Button::SetOnSingleTap(std::function<void()> callback) {
	on_single_tap_ = callback;
}
// SetOnDoubleTap removed
void Button::SetOnLongPress(std::function<void()> callback) {
	on_long_press_ = callback;
}

}  // namespace ui
}  // namespace brain
