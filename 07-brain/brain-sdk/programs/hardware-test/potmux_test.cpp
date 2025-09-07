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
	printf("Pot multiplexer tests\n");
	printf("============================\n\n");
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

	// Thresholds for pot range detection (with ADC tolerance)
	constexpr uint16_t MIN_THRESHOLD = 3;  // Close to 0
	constexpr uint16_t MAX_THRESHOLD = 124;	 // Close to 127

	// Track completion state for each pot
	bool pot_min_reached[NUM_POTS] = {false};
	bool pot_max_reached[NUM_POTS] = {false};
	bool all_pots_tested = false;

	printf("\r\n=== POT CALIBRATION TEST ===\r\n");
	printf(
		"Please turn each pot fully counter-clockwise (minimum), then fully clockwise "
		"(maximum).\r\n");
	printf("Pot range: 0-127 (min threshold: %u, max threshold: %u)\r\n\r\n", MIN_THRESHOLD,
		MAX_THRESHOLD);

	while (!all_pots_tested) {
		pot_mux.scan();

		// Check current values and update status
		for (uint8_t j = 0; j < NUM_POTS; ++j) {
			uint16_t val = pot_mux.get(j);

			// Check if pot reached minimum
			if (!pot_min_reached[j] && val <= MIN_THRESHOLD) {
				pot_min_reached[j] = true;
				printf("✓ Pot %d: Minimum reached (value: %u)\r\n", j, val);
			}

			// Check if pot reached maximum (only after minimum was reached)
			if (pot_min_reached[j] && !pot_max_reached[j] && val >= MAX_THRESHOLD) {
				pot_max_reached[j] = true;
				printf("✓ Pot %d: Maximum reached (value: %u) - COMPLETE!\r\n", j, val);
			}
		}

		// Display current status
		printf("Status: ");
		for (uint8_t j = 0; j < NUM_POTS; ++j) {
			printf("Pot%d:", j);
			if (pot_max_reached[j]) {
				printf("DONE ");
			} else if (pot_min_reached[j]) {
				printf("MIN✓ ");
			} else {
				printf("--- ");
			}
		}

		// Show current values
		printf("| Values: ");
		for (uint8_t j = 0; j < NUM_POTS; ++j) {
			uint16_t val = pot_mux.get(j);
			printf("%u ", val);
		}
		printf("\r");

		// Check if all pots are complete
		all_pots_tested = true;
		for (uint8_t j = 0; j < NUM_POTS; ++j) {
			if (!pot_max_reached[j]) {
				all_pots_tested = false;
				break;
			}
		}

		sleep_ms(100);
	}

	printf("\r\n\r\n🎉 All pots successfully tested!\r\n");
	printf("PotMultiplexer test finished\r\n-----\r\n");
}
