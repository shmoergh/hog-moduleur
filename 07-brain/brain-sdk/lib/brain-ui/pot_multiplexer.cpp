// pot_multiplexer.cpp
// Implementation for multiplexed potentiometer reader using 74HC4051
// Handles ADC sampling, channel switching, and change detection for Brain module
#include "brain-ui/pot_multiplexer.h"

#include <hardware/adc.h>
#include <hardware/gpio.h>
#include <pico/stdlib.h>

#include "brain-common/brain-gpio-setup.h"

namespace brain::ui {

PotMultiplexerConfig createDefaultConfig(uint8_t num_pots, uint8_t output_resolution) {
	PotMultiplexerConfig cfg = {};
	cfg.adc_gpio = GPIO_BRAIN_POTMUX_ADC;
	cfg.s0_gpio = GPIO_BRAIN_POTMUX_S0;
	cfg.s1_gpio = GPIO_BRAIN_POTMUX_S1;
	cfg.num_pots = (num_pots > 3) ? 3 : num_pots;  // Brain module has 3 pots
	for (int i = 0; i < cfg.num_pots; ++i) {
		cfg.channel_map[i] = i;	 // Direct mapping: pot 0 -> channel 0, etc.
	}
	cfg.output_resolution = output_resolution;
	cfg.settling_delay_us = 200;  // Reasonable default for 74HC4051
	cfg.samples_per_read = 6;  // Good balance of stability vs speed
	cfg.change_threshold = 1;  // Sensitive change detection
	return cfg;
}

PotMultiplexer::PotMultiplexer() {
	for (int i = 0; i < kMaxPots; ++i) {
		last_values_[i] = 0;
	}
}

void PotMultiplexer::init(const PotMultiplexerConfig& cfg) {
	config_ = cfg;
	// Ensure num_pots doesn't exceed our array size
	if (config_.num_pots > kMaxPots) {
		config_.num_pots = kMaxPots;
	}

	adc_init();
	gpio_init(cfg.s0_gpio);
	gpio_set_dir(cfg.s0_gpio, GPIO_OUT);
	gpio_put(cfg.s0_gpio, 0);
	gpio_init(cfg.s1_gpio);
	gpio_set_dir(cfg.s1_gpio, GPIO_OUT);
	gpio_put(cfg.s1_gpio, 0);
	adc_gpio_init(cfg.adc_gpio);
	// Select ADC input (Pico SDK: ADC input = GPIO - 26)
	adc_select_input(cfg.adc_gpio - 26);
	// Small guard delay
	busy_wait_us_32(cfg.settling_delay_us);
}

void PotMultiplexer::setMuxChannel(uint8_t ch) {
	ch &= 0x03;
	gpio_put(config_.s0_gpio, ch & 0x01);
	gpio_put(config_.s1_gpio, (ch >> 1) & 0x01);
}

uint16_t PotMultiplexer::readChannelOnce(uint8_t ch) {
	setMuxChannel(ch);
	// Reselect ADC input to ensure proper synchronization
	adc_select_input(config_.adc_gpio - 26);

	// Allow more settling time (minimum 100us is typical for many MUXs)
	busy_wait_us_32(config_.settling_delay_us > 100 ? config_.settling_delay_us : 100);

	// Discard multiple samples to ensure ADC has settled
	for (int i = 0; i < 3; i++) {
		(void) adc_read();
	}

	// Take actual readings
	uint32_t sum = 0;
	uint8_t samples = config_.samples_per_read > 0 ? config_.samples_per_read : 1;
	for (uint8_t i = 0; i < samples; ++i) {
		sum += adc_read();
		// Small delay between samples
		busy_wait_us_32(10);
	}
	return sum / samples;
}

uint16_t PotMultiplexer::getRaw(uint8_t index) {
	if (index >= config_.num_pots || index >= kMaxPots) return 0;
	return readChannelOnce(config_.channel_map[index]);
}

uint16_t PotMultiplexer::get(uint8_t index) {
	if (index >= config_.num_pots || index >= kMaxPots) return 0;

	uint16_t raw = getRaw(index);

	// Map from 12-bit ADC (0-4095) to desired output resolution
	static constexpr uint16_t kAdcMaxValue = 4095;	// 12-bit ADC
	uint16_t output_max = (1 << config_.output_resolution) - 1;

	return (raw * output_max) / kAdcMaxValue;
}

void PotMultiplexer::scan() {
	for (uint8_t i = 0; i < config_.num_pots && i < kMaxPots; ++i) {
		uint16_t val = get(i);
		if (val > last_values_[i] + config_.change_threshold ||
			val + config_.change_threshold < last_values_[i]) {
			last_values_[i] = val;
			if (on_change_) {
				on_change_(i, val);
			}
		}
	}
}

void PotMultiplexer::setOnChange(std::function<void(uint8_t, uint16_t)> cb) {
	on_change_ = cb;
}

}  // namespace brain::ui
