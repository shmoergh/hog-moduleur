// Test function for Brain-UI Button component functionality.
// Demonstrates button press/release, single tap, and long press detection.
#include "brain-ui/button.h"

#include <cstdio>

#include "brain-ui/led.h"
#include "pico/stdlib.h"

// Test GPIO pins - these can be changed to match actual hardware
static constexpr uint kButtonPin = 18;
static constexpr uint kLedPin = 10;

void testButton() {
	printf("2. Button tests\r\n");
	brain::ui::Led led(kLedPin);
	led.init();
	brain::ui::Button button(kButtonPin);
	button.init(true);
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

	for (int i = 0; i < 50; ++i) {
		button.update();
		led.update();
		sleep_ms(100);
	}
	printf("Button test finished\r\n-----\r\n");
}
