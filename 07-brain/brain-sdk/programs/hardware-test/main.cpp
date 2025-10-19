
#include <pico/stdio.h>
#include <pico/stdlib.h>

#include <iostream>

// Function declarations
void testLED();
void testButton();
void testPotMux();
void testPulse();
void testMidi();
void testAudioCvOut();
void testAudioCvIn();

int main() {
	stdio_init_all();
	printf("---== Hog Moduleur Brain hardware test ==---\r\n\r\n");
	printf("Running all tests in continuous loop. Press Ctrl+C to stop.\r\n\r\n");

	// while (true) {
	printf("\r\n=== Starting test iteration ===\r\n");

	// printf("\r\n-> Running LED test...\r\n");
	// testLED();

	// printf("\r\n-> Running button test...\r\n");
	// testButton();

	// printf("\r\n-> Running potentiometer multiplexer test...\r\n");
	// testPotMux();

	// printf("\r\n-> Running pulse test...\r\n");
	// testPulse();

	// printf("\r\n-> Running MIDI parser test...\r\n");
	// testMidi();

	printf("\r\n-> Running Audio CV Out test...\r\n");
	testAudioCvOut();

	// printf("\r\n-> Running Audio CV In test...\r\n");
	// testAudioCvIn();

	printf("\r\n=== Test iteration complete ===\r\n");
	sleep_ms(1000);	 // 1 second delay between iterations
	// }

	return 0;  // This will never be reached
}
