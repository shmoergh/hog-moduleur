#include <pico/stdio.h>
#include <pico/stdlib.h>

#include <iostream>

#include "brain-ui/button.h"
#include "brain-ui/led.h"
#include "brain-ui/pot.h"

const uint LED_PIN = 10;
const uint BUTTON_PIN = 15;	 // Change to your button GPIO
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

	/**
	 *  1. LED tests
	 * ---------------------------------------------------------------------- */
	brain::ui::Led led(LED_PIN);
	led.init();
	led.setOnStateChange([](bool on) { printf("LED state changed: %s\n", on ? "ON" : "OFF"); });
	led.setOnBlinkEnd([]() { printf("LED blink finished\n"); });

	// Test: turn LED on for 500ms, then off
	led.on();
	sleep_ms(500);
	led.off();
	sleep_ms(500);

	// Test: set brightness to half for 500ms
	led.setBrightness(128);
	sleep_ms(500);
	led.setBrightness(0);
	sleep_ms(500);

	// Test: finite blink (3 times, 200ms interval)
	led.blink(3, 200);
	while (led.isOn() || led.isOn() == false) {
		led.update();
		sleep_ms(5);
		if (!led.isOn() && !led.isOn()) break;	// Wait for blink to finish
	}

	// Test: constant blink (indefinite, 100ms interval)
	led.startBlink(100);
	uint blink_duration = 1000;	 // ms
	absolute_time_t blink_start = get_absolute_time();
	while (absolute_time_diff_us(blink_start, get_absolute_time()) / 1000 < blink_duration) {
		led.update();
		sleep_ms(5);
	}
	led.stopBlink();
	sleep_ms(500);

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
	brain::ui::Pot pot(POT_PIN, 7);	 // 10-bit resolution for demo
	pot.init();
	pot.setOnChangeCallback([](uint16_t value) { printf("Pot value changed: %u\n", value); });

	/**
	 * Main loop
	 * ---------------------------------------------------------------------- */
	while (true) {
		button.update();
		pot.update();
		led.update();
		sleep_ms(5);  // Poll every 5ms
	}
	return 0;
}
