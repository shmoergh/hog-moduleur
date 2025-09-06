#include "brain-ui/led.h"

#include <hardware/pwm.h>

namespace brain {
namespace ui {

Led::Led(uint gpio_pin) :
	gpio_pin_(gpio_pin),
	brightness_(255),
	state_(false),
	blinking_(false),
	constant_blink_(false),
	blink_times_(0),
	blink_interval_ms_(0),
	blink_count_(0),
	last_blink_time_(0) {}

void Led::init() {
	gpio_set_function(gpio_pin_, GPIO_FUNC_PWM);
	uint slice = pwm_gpio_to_slice_num(gpio_pin_);
	pwm_set_wrap(slice, 255);
	pwm_set_enabled(slice, true);
	setBrightness(0);
	state_ = false;
}

void Led::on() {
	setBrightness(255);
	state_ = true;
	if (on_state_change_) on_state_change_(true);
}

void Led::off() {
	setBrightness(0);
	state_ = false;
	if (on_state_change_) on_state_change_(false);
}

void Led::setBrightness(uint8_t value) {
	brightness_ = value;
	uint slice = pwm_gpio_to_slice_num(gpio_pin_);
	pwm_set_gpio_level(gpio_pin_, brightness_);
	state_ = (brightness_ > 0);
	if (on_state_change_) on_state_change_(state_);
}

void Led::blink(uint times, uint interval_ms) {
	blinking_ = true;
	constant_blink_ = false;
	blink_times_ = times;
	blink_interval_ms_ = interval_ms;
	blink_count_ = 0;
	last_blink_time_ = get_absolute_time();
}

void Led::startBlink(uint interval_ms) {
	blinking_ = true;
	constant_blink_ = true;
	blink_interval_ms_ = interval_ms;
	last_blink_time_ = get_absolute_time();
}

void Led::stopBlink() {
	blinking_ = false;
	constant_blink_ = false;
	setBrightness(0);
	if (on_blink_end_) on_blink_end_();
}

void Led::update() {
	if (!blinking_) return;
	absolute_time_t now = get_absolute_time();
	if (absolute_time_diff_us(last_blink_time_, now) / 1000 >= blink_interval_ms_) {
		last_blink_time_ = now;
		if (state_) {
			off();
			if (!constant_blink_) blink_count_++;
		} else {
			on();
		}
		if (!constant_blink_ && blink_count_ >= blink_times_) {
			stopBlink();
		}
	}
}

void Led::setOnStateChange(std::function<void(bool)> callback) {
	on_state_change_ = callback;
}

void Led::setOnBlinkEnd(std::function<void()> callback) {
	on_blink_end_ = callback;
}

bool Led::isOn() const {
	return state_;
}

}  // namespace ui
}  // namespace brain
