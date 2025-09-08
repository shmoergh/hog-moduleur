#include "brain-ui/led.h"

#include <pico/stdlib.h>

#include <cstdio>

#include "brain-common/brain-gpio-setup.h"

constexpr uint LED_PIN = GPIO_BRAIN_LED_1;

void testLED() {
	printf("LED tests\n");
	printf("============================\n\n");
	brain::ui::Led led(LED_PIN);
	led.init();
	led.setOnStateChange([](bool on) { printf("LED state changed: %s\n", on ? "ON" : "OFF"); });
	led.setOnBlinkEnd([]() { printf("LED blink finished\n"); });

	printf("1.1 Turn LED on for 500ms, then off\r\n");
	led.on();
	sleep_ms(500);
	led.off();
	sleep_ms(200);

	printf("1.2 Set brightness to half for 2s\r\n");
	led.setBrightness(128);
	sleep_ms(2000);
	led.setBrightness(0);
	sleep_ms(200);

	printf("1.3 Finite blink (3 times, 200ms interval)\r\n");
	led.blink(3, 200);
	while (led.isBlinking()) {
		led.update();
		sleep_ms(5);
	}
	sleep_ms(200);

	printf("1.4 Duration blink (2s, 100ms interval)\r\n");
	led.blinkDuration(2000, 100);
	while (led.isBlinking()) {
		led.update();
		sleep_ms(5);
	}
	sleep_ms(200);

	printf("1.5 Constant blink (for 2s, 200ms interval)\r\n");
	led.startBlink(200);
	uint blink_duration = 2000;
	absolute_time_t blink_start = get_absolute_time();
	while (absolute_time_diff_us(blink_start, get_absolute_time()) / 1000 < blink_duration) {
		led.update();
		sleep_ms(5);
	}
	led.stopBlink();
	sleep_ms(200);
	printf("LED test finished\r\n-----\r\n");
}
