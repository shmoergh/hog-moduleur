#include "brain-ui/pot.h"

#include <hardware/adc.h>

namespace brain {
namespace ui {

Pot::Pot(uint gpio_pin, uint8_t resolution_bits) :
	gpio_pin_(gpio_pin),
	resolution_bits_(resolution_bits),
	raw_value_(0),
	scaled_value_(0),
	last_value_(0),
	on_change_callback_(nullptr) {}

void Pot::init() {
	adc_init();
	adc_gpio_init(gpio_pin_);
	// Select ADC input channel based on pin (assume pin matches ADC channel)
	adc_select_input(gpio_pin_ - 26);  // For RP2040: ADC0=GPIO26, ADC1=GPIO27, etc.
	raw_value_ = 0;
	scaled_value_ = 0;
	last_value_ = 0;
}

void Pot::update() {
	raw_value_ = adc_read();
	// Add to smoothing buffer
	smoothing_buffer_[smoothing_index_++] = raw_value_;
	if (smoothing_index_ >= kSmoothingWindow) {
		smoothing_index_ = 0;
		smoothing_filled_ = true;
	}
	uint16_t smoothed = getSmoothedValue();
	uint16_t max_adc = (1 << 12) - 1;  // RP2040 ADC is 12-bit
	uint16_t max_res = (1 << resolution_bits_) - 1;
	scaled_value_ = (smoothed * max_res) / max_adc;
	if (scaled_value_ != last_value_) {
		last_value_ = scaled_value_;
		if (on_change_callback_) {
			on_change_callback_(scaled_value_);
		}
	}
}

void Pot::setOnChangeCallback(std::function<void(uint16_t)> callback) {
	on_change_callback_ = callback;
}

uint16_t Pot::getValue() const {
	return scaled_value_;
}

uint16_t Pot::getSmoothedValue() const {
	size_t count = smoothing_filled_ ? kSmoothingWindow : smoothing_index_;
	uint32_t sum = 0;
	for (size_t i = 0; i < count; ++i) {
		sum += smoothing_buffer_[i];
	}
	return count ? (sum / count) : 0;
}

}  // namespace ui
}  // namespace brain
