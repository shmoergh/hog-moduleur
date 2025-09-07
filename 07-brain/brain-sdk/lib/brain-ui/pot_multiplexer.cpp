// PotMultiplexer.cpp
// Implementation for multiplexed potentiometer reader
#include "brain-ui/pot_multiplexer.h"

#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "pico/stdlib.h"

namespace brain_ui {

PotMultiplexer::PotMultiplexer() {
	for (int i = 0; i < MAX_POTS; ++i) {
		last_values_[i] = 0;
	}
}

void PotMultiplexer::Init(const PotMultiplexerConfig& cfg) {
	config_ = cfg;
	// Ensure num_pots doesn't exceed our array size
	if (config_.num_pots > MAX_POTS) {
		config_.num_pots = MAX_POTS;
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

void PotMultiplexer::SetMuxChannel(uint8_t ch) {
	ch &= 0x03;
	gpio_put(config_.s0_gpio, ch & 0x01);
	gpio_put(config_.s1_gpio, (ch >> 1) & 0x01);
}

uint16_t PotMultiplexer::ReadChannelOnce(uint8_t ch) {
	SetMuxChannel(ch);
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

uint16_t PotMultiplexer::GetRaw(uint8_t index) {
	if (index >= config_.num_pots || index >= MAX_POTS) return 0;
	return ReadChannelOnce(config_.channel_map[index]);
}

uint16_t PotMultiplexer::Get(uint8_t index) {
	if (index >= config_.num_pots || index >= MAX_POTS) return 0;

	uint16_t raw = GetRaw(index);

	// Map from 12-bit ADC (0-4095) to desired output resolution
	static constexpr uint16_t kAdcMaxValue = 4095;	// 12-bit ADC
	uint16_t output_max = (1 << config_.output_resolution) - 1;

	return (raw * output_max) / kAdcMaxValue;
}

void PotMultiplexer::Scan() {
	for (uint8_t i = 0; i < config_.num_pots && i < MAX_POTS; ++i) {
		uint16_t val = Get(i);
		if (val > last_values_[i] + config_.change_threshold ||
			val + config_.change_threshold < last_values_[i]) {
			last_values_[i] = val;
			if (on_change_) {
				on_change_(i, val);
			}
		}
	}
}

void PotMultiplexer::SetOnChange(std::function<void(uint8_t, uint16_t)> cb) {
	on_change_ = cb;
}

}  // namespace brain_ui
