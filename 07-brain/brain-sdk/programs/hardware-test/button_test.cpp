// Test function for Brain-UI Button component functionality.
// Demonstrates button press/release, single tap, and long press detection.
#include "button.h"

#include <cstdio>

#include "brain-common/brain-common.h"
#include "led.h"
#include "pico/stdlib.h"

void testButton() {
	printf("Button tests\n");
	printf("============================\n\n");
	brain::ui::Led led(BRAIN_LED_1);
	led.init();
	brain::ui::Button button(BRAIN_BUTTON_1);
	button.init(true);

	bool singleTapDetected = false;
	bool longPressDetected = false;

	button.setOnPress([&]() {
		printf("Button pressed!\n");
		led.on();
	});
	button.setOnRelease([&]() {
		printf("Button released!\n");
		led.off();
	});
	button.setOnSingleTap([&]() {
		printf("Single tap detected!\n");
		singleTapDetected = true;
	});
	button.setOnLongPress([&]() {
		printf("Long press detected!\n");
		longPressDetected = true;
	});

	printf("Please perform a SINGLE TAP on the button to continue...\n");
	while (!singleTapDetected) {
		button.update();
		led.update();
		sleep_ms(10);
	}

	printf(
		"Great! Now please perform a LONG PRESS (hold for ~1 second) on the button to "
		"continue...\n");
	while (!longPressDetected) {
		button.update();
		led.update();
		sleep_ms(10);
	}

	printf("Button test finished successfully!\r\n-----\r\n");
}
