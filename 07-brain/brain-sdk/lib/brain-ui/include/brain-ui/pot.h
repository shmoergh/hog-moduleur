#pragma once
#include <pico/stdlib.h>

#include <functional>

namespace brain {
namespace ui {
/**
 * @brief Potentiometer (analog input) handler with scaling and change detection.
 */
class Pot {
	public:
	Pot(uint gpio_pin, uint8_t resolution_bits = 12);

	void init();
	void update();

	void setOnChangeCallback(std::function<void(uint16_t)> callback);
	uint16_t getValue() const;

	private:
	uint gpio_pin_;
	uint8_t resolution_bits_;
	uint16_t raw_value_;
	uint16_t scaled_value_;
	uint16_t last_value_;
	std::function<void(uint16_t)> on_change_callback_;

	// Smoothing buffer
	static constexpr size_t kSmoothingWindow = 8;
	uint16_t smoothing_buffer_[kSmoothingWindow] = {0};
	size_t smoothing_index_ = 0;
	bool smoothing_filled_ = false;
	uint16_t getSmoothedValue() const;
};

}  // namespace ui
}  // namespace brain
