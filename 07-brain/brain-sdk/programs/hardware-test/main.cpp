#include <pico/stdio.h>
#include <pico/stdlib.h>

#include <iostream>

#include "brain-ui/button.h"
#include "brain-ui/led.h"
#include "brain-ui/pot.h"

const uint LED_PIN = 10;
const uint BUTTON_PIN = 18;	 // Change to your button GPIO
const uint POT_PIN = 26;  // Change to your pot GPIO (ADC0=GPIO26)

/**
 * Testing Brain hardware
 *
 * 1. LED tests
 * 2. Button tests
 * 3. Pot tests
 */

int main() {
	stdio_init_all();

	printf("---== Hog Moduleur Brain hardware test ==---\r\n");
	printf("\r\n");

	/**
	 *  1. LED tests
	 * ---------------------------------------------------------------------- */
	printf("1. LED tests");
	sleep_ms(2000);
	brain::ui::Led led(LED_PIN);
	led.init();
	led.setOnStateChange([](bool on) { printf("LED state changed: %s\n", on ? "ON" : "OFF"); });
	led.setOnBlinkEnd([]() { printf("LED blink finished\n"); });
	printf("\r\n");

	// Test: turn LED on for 500ms, then off
	printf("1.1 Turn LED on for 500ms, then off\r\n");
	led.on();
	sleep_ms(500);
	led.off();
	sleep_ms(2000);
	printf("\r\n");

	// Test: set brightness to half for 2s
	printf("1.2 Set brightness to half for 2s\r\n");
	led.setBrightness(128);
	sleep_ms(2000);
	led.setBrightness(0);
	sleep_ms(2000);
	printf("\r\n");

	// Test: finite blink (3 times, 200ms interval)
	printf("1.3 Finite blink (3 times, 200ms interval)\r\n");
	led.blink(3, 200);
	while (led.isBlinking()) {
		led.update();
		sleep_ms(5);
	}
	sleep_ms(2000);
	printf("\r\n");

	// Test: duration blink (blink for 2s, 100ms interval)
	printf("1.4 Duration blink (2s, 100ms interval)\r\n");
	led.blinkDuration(2000, 100);
	while (led.isBlinking()) {
		led.update();
		sleep_ms(5);
	}
	sleep_ms(2000);
	printf("\r\n");

	// Test: constant blink (for 2s, 200ms interval)
	printf("1.5 Constant blink (for 2s, 200ms interval)\r\n");
	led.startBlink(200);
	uint blink_duration = 2000;	 // ms
	absolute_time_t blink_start = get_absolute_time();
	while (absolute_time_diff_us(blink_start, get_absolute_time()) / 1000 < blink_duration) {
		led.update();
		sleep_ms(5);
	}
	led.stopBlink();
	sleep_ms(2000);
	printf("\r\n");
	printf("LED test finished\r\n");
	printf("-----\r\n");

	/**
	 * 2. Button tests
	 * ---------------------------------------------------------------------- */
	brain::ui::Button button(BUTTON_PIN);
	button.init(true);	// Use pull-up

	button.setOnPress([&]() {
		printf("Button pressed!\n");
		led.on();
	});
	button.setOnRelease([&]() {
		printf("Button released!\n");
		led.off();
	});
	button.setOnSingleTap([]() { printf("Single tap detected!\n"); });
	button.setOnLongPress([]() { printf("Long press detected!\n"); });

	/**
	 * 3. Pot tests
	 * ---------------------------------------------------------------------- */
	// brain::ui::Pot pot(POT_PIN, 7);	 // 10-bit resolution for demo
	// pot.init();
	// pot.setOnChangeCallback([](uint16_t value) { printf("Pot value changed: %u\n", value); });

	/**
	 * Main loop
	 * ---------------------------------------------------------------------- */
	while (true) {
		button.update();
		// pot.update();
		led.update();
		sleep_ms(5);  // Poll every 5ms
	}
	return 0;
}
