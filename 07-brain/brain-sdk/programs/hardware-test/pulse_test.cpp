// Test program for Brain-IO Pulse component functionality.
// Demonstrates digital I/O, edge detection, and callback system.
// Uses GPIO pins 2 (input) and 3 (output) for testing.

#include "brain-io/pulse.h"

#include <cstdio>

#include "pico/stdlib.h"

// Test GPIO pins - these can be changed to match actual hardware
static constexpr int kTestInputGpio = 2;
static constexpr int kTestOutputGpio = 3;

void test_pulse() {
	printf("Brain-IO Pulse Component Test\n");
	printf("============================\n\n");

	// Create pulse instance
	brain::io::Pulse pulse(kTestInputGpio, kTestOutputGpio);

	// Initialize the pulse component
	pulse.begin();
	printf("Pulse component initialized\n");

	// Test output functionality
	printf("Testing output control...\n");

	pulse.set(true);
	printf("Output set to ON (active): %s\n", pulse.get() ? "true" : "false");

	pulse.set(false);
	printf("Output set to OFF (idle): %s\n", pulse.get() ? "true" : "false");

	// Test idempotent behavior
	pulse.set(false);
	printf("Set OFF again (should not change): %s\n", pulse.get() ? "true" : "false");

	// Set up callbacks for edge detection
	printf("\nSetting up edge callbacks...\n");

	pulse.onRise([] { printf("Rising edge detected!\n"); });

	pulse.onFall([] { printf("Falling edge detected!\n"); });

	// Test input reading
	printf("\nTesting input reading...\n");
	printf("Current logical input: %s\n", pulse.read() ? "HIGH" : "LOW");
	printf("Current raw input: %s\n", pulse.readRaw() ? "HIGH" : "LOW");

	// Enable glitch filtering
	pulse.setInputGlitchFilterUs(1000);	 // 1ms filter
	printf("Glitch filter enabled (1ms)\n");

	// Test edge detection for a few seconds
	printf("\nTesting edge detection for 5 seconds...\n");
	printf("Connect/disconnect signal to input pin %d to test edge detection\n", kTestInputGpio);

	uint32_t start_time = to_ms_since_boot(get_absolute_time());
	uint32_t last_print = start_time;

	while ((to_ms_since_boot(get_absolute_time()) - start_time) < 5000) {
		// Poll for edges
		pulse.poll();

		// Print status every second
		uint32_t now = to_ms_since_boot(get_absolute_time());
		if (now - last_print >= 1000) {
			printf("Input: %s, Output: %s\n", pulse.read() ? "HIGH" : "LOW",
				pulse.get() ? "ON" : "OFF");
			last_print = now;
		}

		sleep_ms(10);
	}

	// Clean up
	pulse.end();
	printf("\nPulse component cleaned up\n");
	printf("Pulse test completed\n\n");
}
