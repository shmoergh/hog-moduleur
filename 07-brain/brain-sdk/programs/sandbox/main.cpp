#include <pico/stdio.h>
#include <pico/stdlib.h>

#include <iostream>

int main() {
	stdio_init_all();

	while (true) {
		sleep_ms(5);  // Poll every 5ms
	}
	return 0;
}
