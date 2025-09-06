#include <pico/stdio.h>
#include <pico/stdlib.h>

#include <iostream>

#include "brain-ui/button.h"
#include "brain-ui/pot.h"

const uint LED_PIN = PICO_DEFAULT_LED_PIN;
const uint BUTTON_PIN = 15;	 // Change to your button GPIO
const uint POT_PIN = 26;  // Change to your pot GPIO (ADC0=GPIO26)

int main() {
	stdio_init_all();
	gpio_init(LED_PIN);
	gpio_set_dir(LED_PIN, GPIO_OUT);

	/**
	 * Button tests
	 */
	brain::ui::Button button(BUTTON_PIN);
	button.init(true);	// Use pull-up

	button.setOnPress([]() {
		printf("Button pressed!\n");
		gpio_put(LED_PIN, 1);
	});
	button.setOnRelease([]() {
		printf("Button released!\n");
		gpio_put(LED_PIN, 0);
	});
	button.setOnSingleTap([]() { printf("Single tap detected!\n"); });
	button.setOnLongPress([]() { printf("Long press detected!\n"); });

	/**
	 * Pot tests
	 */
	brain::ui::Pot pot(POT_PIN, 7);	 // 10-bit resolution for demo
	pot.init();
	pot.setOnChangeCallback([](uint16_t value) { printf("Pot value changed: %u\n", value); });

	/**
	 * Main loop
	 */
	while (true) {
		button.update();
		pot.update();
		sleep_ms(5);  // Poll every 5ms
	}
	return 0;
}
