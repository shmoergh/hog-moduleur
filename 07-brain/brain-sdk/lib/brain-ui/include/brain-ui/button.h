#pragma once
#include <pico/stdlib.h>

#include <functional>

namespace brain {
namespace ui {

/**
 * @brief Button input handler with debounce, long press, and double-tap detection.
 */
class Button {
	public:
	Button(uint gpio_pin, uint32_t debounce_ms = 50, uint32_t long_press_ms = 500);

	void init(bool pull_up = true);
	void update();

	void setOnPress(std::function<void()> callback);
	void setOnRelease(std::function<void()> callback);
	void setOnSingleTap(std::function<void()> callback);
	void setOnLongPress(std::function<void()> callback);

	private:
	uint gpio_pin_;
	bool is_pressed_;
	absolute_time_t last_press_time_;
	absolute_time_t last_release_time_;
	absolute_time_t last_tap_time_;
	uint32_t debounce_ms_;
	uint32_t long_press_ms_;

	std::function<void()> on_press_;
	std::function<void()> on_release_;
	std::function<void()> on_single_tap_;
	std::function<void()> on_long_press_;

	bool long_press_triggered_;
	// double tap members removed
};

}  // namespace ui
}  // namespace brain
