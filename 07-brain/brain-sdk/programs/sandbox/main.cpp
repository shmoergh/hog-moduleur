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

	brain::ui::Button button(BUTTON_PIN);
	button.Init(true);	// Use pull-up

	button.SetOnPress([]() {
		printf("Button pressed!\n");
		gpio_put(LED_PIN, 1);
	});
	button.SetOnRelease([]() {
		printf("Button released!\n");
		gpio_put(LED_PIN, 0);
	});
	button.SetOnSingleTap([]() { printf("Single tap detected!\n"); });
	button.SetOnLongPress([]() { printf("Long press detected!\n"); });

	brain::ui::Pot pot(POT_PIN, 7);	 // 10-bit resolution for demo
	pot.Init();
	pot.SetOnChangeCallback([](uint16_t value) { printf("Pot value changed: %u\n", value); });

	while (true) {
		button.Update();
		pot.Update();
		sleep_ms(5);  // Poll every 5ms
	}
	return 0;
}
