#include <pico/stdlib.h>

const uint LED_PIN = PICO_DEFAULT_LED_PIN;

int main() {
	stdio_init_all();
	gpio_init(LED_PIN);
	gpio_set_dir(LED_PIN, GPIO_OUT);
	while (true) {
		gpio_put(LED_PIN, 1);
		sleep_ms(500);
		gpio_put(LED_PIN, 0);
		sleep_ms(500);
	}
	return 0;
}
