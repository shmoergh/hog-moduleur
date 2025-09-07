
#include <pico/stdio.h>
#include <pico/stdlib.h>

#include <iostream>

#include "button_test.h"
#include "led_test.h"
#include "potmux_test.h"

// Function declarations
void test_pulse();

int main() {
	stdio_init_all();
	printf("---== Hog Moduleur Brain hardware test ==---\r\n\r\n");
	printf("Running all tests in continuous loop. Press Ctrl+C to stop.\r\n\r\n");

	while (true) {
		printf("\r\n=== Starting test iteration ===\r\n");

		// printf("\r\n-> Running LED test...\r\n");
		RunLedTest();

		// printf("\r\n-> Running button test...\r\n");
		// RunButtonTest();

		printf("\r\n-> Running potentiometer multiplexer test...\r\n");
		RunPotmuxTest();

		printf("\r\n-> Running pulse test...\r\n");
		test_pulse();

		printf("\r\n=== Test iteration complete ===\r\n");
		// sleep_ms(1000);	 // 1 second delay between iterations
	}

	return 0;  // This will never be reached
}
