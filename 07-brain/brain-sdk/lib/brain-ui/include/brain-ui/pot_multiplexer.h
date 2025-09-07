// PotMultiplexer.h
// Multiplexed potentiometer reader for Hog Moduleur Brain
// See ai/todos/07-pot-multiplexer-todo.md for requirements
#pragma once

#include <cstdint>
#include <functional>

namespace brain_ui {

static constexpr uint8_t MAX_POTS = 4;	// 4-channel multiplexer

struct PotMultiplexerConfig {
	uint8_t adc_gpio;
	uint8_t s0_gpio;
	uint8_t s1_gpio;
	uint8_t num_pots;
	uint8_t channel_map[MAX_POTS];
	uint8_t output_resolution;	// Output resolution in bits (e.g., 7 for 0-127)
	uint32_t settling_delay_us;
	uint8_t samples_per_read;
	uint16_t change_threshold;
};

class PotMultiplexer {
	public:
	PotMultiplexer();
	void Init(const PotMultiplexerConfig& cfg);
	void Scan();
	uint16_t Get(uint8_t index);
	uint16_t GetRaw(uint8_t index);
	void SetOnChange(std::function<void(uint8_t, uint16_t)> cb);

	private:
	void SetMuxChannel(uint8_t ch);
	uint16_t ReadChannelOnce(uint8_t ch);

	PotMultiplexerConfig config_;
	uint16_t last_values_[MAX_POTS];
	std::function<void(uint8_t, uint16_t)> on_change_;
};

}  // namespace brain_ui
