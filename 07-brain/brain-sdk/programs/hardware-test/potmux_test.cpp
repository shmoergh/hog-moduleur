#include <pico/stdlib.h>

#include <cstdio>

#include "brain-ui/pot_multiplexer.h"

constexpr uint8_t POT_ADC_GPIO = 26;
constexpr uint8_t POT_S0_GPIO = 16;
constexpr uint8_t POT_S1_GPIO = 17;
constexpr uint8_t NUM_POTS = 3;
constexpr uint8_t POT_CHANNEL_MAP[NUM_POTS] = {0, 1, 2};

brain::ui::PotMultiplexer pot_mux;

void pot_change_cb(uint8_t idx, uint16_t val) {
	printf("Pot %d changed: %u\r\n", idx, val);
}

void testPotMux() {
	printf("3. PotMultiplexer tests\r\n");
	brain::ui::PotMultiplexerConfig pot_cfg = {};
	pot_cfg.adc_gpio = POT_ADC_GPIO;
	pot_cfg.s0_gpio = POT_S0_GPIO;
	pot_cfg.s1_gpio = POT_S1_GPIO;
	pot_cfg.num_pots = NUM_POTS;
	for (int i = 0; i < NUM_POTS; ++i) pot_cfg.channel_map[i] = POT_CHANNEL_MAP[i];
	pot_cfg.output_resolution = 7;	// 0-127 range (MIDI-like)
	pot_cfg.settling_delay_us = 200;
	pot_cfg.samples_per_read = 6;
	pot_cfg.change_threshold = 1;

	pot_mux.init(pot_cfg);
	pot_mux.setOnChange(pot_change_cb);

	for (int i = 0; i < 50; ++i) {
		pot_mux.scan();
		for (uint8_t j = 0; j < NUM_POTS; ++j) {
			uint16_t val = pot_mux.get(j);
			printf("Pot %d value: %u\t", j, val);
		}
		printf("\r\n");
		sleep_ms(100);
	}
	printf("PotMultiplexer test finished\r\n-----\r\n");
}
