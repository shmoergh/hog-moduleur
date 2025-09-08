// Test program for Brain-IO Pulse component functionality.
// Demonstrates digital I/O, edge detection, and callback system.
// Uses GPIO pins 2 (input) and 3 (output) for testing.

#include "brain-io/pulse.h"

#include <cstdio>

#include "pico/stdlib.h"
#include "settings.h"

// Test GPIO pins - these can be changed to match actual hardware
static constexpr int TEST_INPUT_GPIO = GPIO_PULSE_INPUT;
static constexpr int TEST_OUTPUT_GPIO = GPIO_PULSE_OUTPUT;

void testPulseInput(brain::io::Pulse& pulse) {
	printf("PHASE 1: PULSE INPUT TEST\n");
	printf("============================\n");
	printf("Hardware setup:\n");
	printf("- Connect a button between GPIO pin %d and GROUND (GND)\n", TEST_INPUT_GPIO);
	printf("- Button should pull GPIO LOW when pressed (hardware inverts this to logical HIGH)\n");
	printf("- No pull-down resistor needed (internal pull-up enabled)\n\n");

	printf("Test procedure:\n");
	printf("- Press and release the button at least 3 times to pass this test\n");
	printf("- You should see 'Button pressed!' and 'Button released!' messages\n");
	printf("- Test will automatically proceed when 3 presses are detected\n\n");

	printf("Press ENTER when hardware is connected and you're ready to start...\n");
	getchar();

	// Counter for button presses
	int button_press_count = 0;

	// Set up callbacks for edge detection
	pulse.onRise([&button_press_count] {
		button_press_count++;
		printf("Button pressed! (Count: %d/3)\n", button_press_count);
	});
	pulse.onFall([] { printf("Button released!\n"); });

	// Enable glitch filtering to debounce the button (or disable for testing)
	pulse.setInputGlitchFilterUs(0);  // Disable glitch filter for testing
	printf("Starting input test (glitch filter disabled for testing)...\n");
	printf("Press and release the button at least 3 times.\n");
	printf("Progress will be shown with each button press...\n\n");

	uint32_t last_status_print = to_ms_since_boot(get_absolute_time());
	bool last_displayed_state = pulse.read();
	printf("Initial input state: %s\n", last_displayed_state ? "HIGH (pressed)" : "LOW (released)");

	while (button_press_count < 3) {
		// Poll for edges
		pulse.poll();

		// Show immediate state changes for debugging
		bool current_state = pulse.read();
		if (current_state != last_displayed_state) {
			printf("State change detected: %s -> %s\n", last_displayed_state ? "HIGH" : "LOW",
				current_state ? "HIGH" : "LOW");
			last_displayed_state = current_state;
		}

		// Print status every 3 seconds
		uint32_t now = to_ms_since_boot(get_absolute_time());
		if (now - last_status_print >= 3000) {
			printf("Waiting for button presses... Current count: %d/3\n", button_press_count);
			printf("Input state: %s\n", pulse.read() ? "HIGH (pressed)" : "LOW (released)");
			last_status_print = now;
		}

		sleep_ms(10);
	}

	printf(
		"\n✓ Input test PASSED! Successfully detected %d button presses.\n\n", button_press_count);
}

void testPulseOutput(brain::io::Pulse& pulse) {
	printf("PHASE 2: PULSE OUTPUT TEST\n");
	printf("============================\n");
	printf("Hardware setup:\n");
	printf("- Connect an oscilloscope or logic analyzer to GPIO pin %d\n", TEST_OUTPUT_GPIO);
	printf("- You should see 10 pulses, each 100ms wide, with 1 second intervals\n");
	printf("- Pulse logic: HIGH = active, LOW = idle\n\n");

	printf("Press ENTER when oscilloscope is connected and you're ready to start...\n");
	getchar();

	printf("Starting output test...\n");
	printf("Sending 10 pulses with 1 second intervals:\n\n");

	for (int i = 1; i <= 10; i++) {
		printf("Pulse %d/10: ", i);

		// Send pulse: HIGH for 100ms, then LOW
		pulse.set(true);
		printf("HIGH");
		sleep_ms(100);

		pulse.set(false);
		printf(" -> LOW\n");

		// Wait 1 second before next pulse (except after the last pulse)
		if (i < 10) {
			printf("Waiting 1 second...\n");
			sleep_ms(1000);
		}
	}

	printf("\nOutput test completed!\n\n");
}

void testPulse() {
	printf("BRAIN-IO PULSE COMPONENT TEST\n");
	printf("============================\n");
	printf("This test validates both input and output functionality\n");
	printf("Test pins: Input=%d, Output=%d\n\n", TEST_INPUT_GPIO, TEST_OUTPUT_GPIO);

	// Create pulse instance
	brain::io::Pulse pulse(TEST_INPUT_GPIO, TEST_OUTPUT_GPIO);

	// Initialize the pulse component
	pulse.begin();
	printf("Pulse component initialized successfully\n\n");

	// Test input functionality
	testPulseInput(pulse);

	// Test output functionality
	testPulseOutput(pulse);

	// Clean up
	pulse.end();
	printf("Pulse component cleaned up\n");
	printf("All tests completed successfully!\n\n");
}
