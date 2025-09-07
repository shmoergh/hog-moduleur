#include "button_test.h"

#include <pico/stdlib.h>

#include <cstdio>

#include "brain-ui/button.h"
#include "brain-ui/led.h"

constexpr uint BUTTON_PIN = 18;
constexpr uint LED_PIN = 10;

void RunButtonTest() {
	printf("2. Button tests\r\n");
	brain::ui::Led led(LED_PIN);
	led.init();
	brain::ui::Button button(BUTTON_PIN);
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
