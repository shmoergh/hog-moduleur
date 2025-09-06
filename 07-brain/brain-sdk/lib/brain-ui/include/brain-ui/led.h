#pragma once
#include <pico/stdlib.h>

#include <functional>

namespace brain {
namespace ui {
/**
 * @brief LED handler for PWM brightness, blinking, and state callbacks.
 */
class Led {
	public:
	Led(uint gpio_pin);
	void init();
	void on();
	void off();
	void setBrightness(uint8_t value);
	void blink(uint times, uint interval_ms);
	void startBlink(uint interval_ms);
	void stopBlink();
	void update();
	void setOnStateChange(std::function<void(bool)> callback);
	void setOnBlinkEnd(std::function<void()> callback);
	bool isOn() const;

	private:
	uint gpio_pin_;
	uint8_t brightness_;
	bool state_;
	bool blinking_;
	bool constant_blink_;
	uint blink_times_;
	uint blink_interval_ms_;
	uint blink_count_;
	absolute_time_t last_blink_time_;
	std::function<void(bool)> on_state_change_;
	std::function<void()> on_blink_end_;
};
}  // namespace ui
}  // namespace brain
