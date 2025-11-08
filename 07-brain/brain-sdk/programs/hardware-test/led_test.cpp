#include "brain-ui/led.h"

#include <pico/stdlib.h>

#include <cstdio>

#include "brain-common/brain-common.h"

constexpr uint LED_PINS[] = {
	BRAIN_LED_1, BRAIN_LED_2, BRAIN_LED_3, BRAIN_LED_4, BRAIN_LED_5, BRAIN_LED_6};
constexpr uint NUM_LEDS = 6;

void testLED() {
	printf("LED tests\n");
	printf("============================\n\n");

	// Initialize all LEDs
	brain::ui::Led leds[NUM_LEDS] = {brain::ui::Led(LED_PINS[0]), brain::ui::Led(LED_PINS[1]),
		brain::ui::Led(LED_PINS[2]), brain::ui::Led(LED_PINS[3]), brain::ui::Led(LED_PINS[4]),
		brain::ui::Led(LED_PINS[5])};

	// Reset all LEDs to off
	printf("Resetting all LEDs to OFF\n");
	for (uint i = 0; i < NUM_LEDS; i++) {
		leds[i].init();
		leds[i].off();
	}
	sleep_ms(500);

	// Test each LED individually
	for (uint i = 0; i < NUM_LEDS; i++) {
		printf("\nTesting LED %d (Pin %d)\n", i + 1, LED_PINS[i]);
		printf("------------------------\n");

		printf("  Turn on for 500ms\n");
		leds[i].on();
		sleep_ms(500);
		leds[i].off();
		sleep_ms(200);

		printf("  Set brightness to half for 500ms\n");
		leds[i].setBrightness(128);
		sleep_ms(500);
		leds[i].setBrightness(0);
		sleep_ms(200);

		printf("  Blink 3 times (200ms interval)\n");
		leds[i].blink(3, 200);
		while (leds[i].isBlinking()) {
			leds[i].update();
			sleep_ms(5);
		}
		sleep_ms(200);
	}

	// Test all LEDs together
	printf("\nTesting all LEDs together\n");
	printf("------------------------\n");

	printf("  All on for 1s\n");
	for (uint i = 0; i < NUM_LEDS; i++) {
		leds[i].on();
	}
	sleep_ms(1000);

	printf("  All off\n");
	for (uint i = 0; i < NUM_LEDS; i++) {
		leds[i].off();
	}
	sleep_ms(500);

	printf("\nLED test finished\n");
	printf("============================\n");
}
