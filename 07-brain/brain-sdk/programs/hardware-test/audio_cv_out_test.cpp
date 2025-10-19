#include <pico/stdlib.h>

#include <cstdio>

#include "brain-io/audio-cv-out.h"

using brain::io::AudioCvOut;
using brain::io::AudioCvOutChannel;
using brain::io::AudioCvOutCoupling;

static void waitForEnter(const char* prompt = "Press Enter to continue...") {
	// printf("%s\r\n", prompt);
	sleep_ms(10000);

	// fflush(stdout);
	// while (true) {
	// 	int ch = getchar_timeout_us(1000 * 1000);
	// 	if (ch == '\n' || ch == '\r') {
	// 		break;
	// 	}
	// }
}

void testAudioCvOut() {
	printf("AudioCvOut Test Starting...\n");

	// Initialize audio CV output with default pins
	static AudioCvOut audio_cv_out;
	if (!audio_cv_out.init()) {
		printf("ERROR: Failed to initialize AudioCvOut\n");
		return;
	}
	printf("AudioCvOut initialized successfully\n");
	waitForEnter();

	// Test voltage sweep on both channels
	printf("Testing voltage sweep (0-10V)...\n");
	for (int i = 0; i <= 5; i++) {
		float voltage = static_cast<float>(i * 2);	// 0, 2, 4, 6, 8, 10V

		// Set voltage on both channels
		if (!audio_cv_out.setVoltage(AudioCvOutChannel::kChannelA, voltage)) {
			printf("ERROR: Failed to set voltage %.1fV on channel A\n", voltage);
		}
		if (!audio_cv_out.setVoltage(AudioCvOutChannel::kChannelB, voltage)) {
			printf("ERROR: Failed to set voltage %.1fV on channel B\n", voltage);
		}

		printf("Set both channels to %.1fV\n", voltage);
		waitForEnter();
	}

	// Test coupling modes
	printf("Testing coupling modes...\n");

	// Set 5V on both channels
	audio_cv_out.setVoltage(AudioCvOutChannel::kChannelA, 5.0f);
	audio_cv_out.setVoltage(AudioCvOutChannel::kChannelB, 5.0f);

	// Test DC coupling (default)
	audio_cv_out.setCoupling(AudioCvOutChannel::kChannelA, AudioCvOutCoupling::kDcCoupled);
	audio_cv_out.setCoupling(AudioCvOutChannel::kChannelB, AudioCvOutCoupling::kDcCoupled);
	printf("Set DC coupling on both channels\n");
	waitForEnter();

	// Test AC coupling
	audio_cv_out.setCoupling(AudioCvOutChannel::kChannelA, AudioCvOutCoupling::kAcCoupled);
	audio_cv_out.setCoupling(AudioCvOutChannel::kChannelB, AudioCvOutCoupling::kAcCoupled);
	printf("AC coupling: outputting ~60Hz square wave (0/10V). Press Enter to stop...\n");

	// Generate a simple ~60Hz square wave until user presses Enter
	{
		const uint32_t half_period_us = 1000000 / (60 * 2);	 // ~8.33 ms
		bool stop = false;
		while (!stop) {
			// High level
			(void) audio_cv_out.setVoltage(AudioCvOutChannel::kChannelA, 10.0f);
			(void) audio_cv_out.setVoltage(AudioCvOutChannel::kChannelB, 10.0f);
			int ch = getchar_timeout_us(half_period_us);
			if (ch == '\n' || ch == '\r') {
				break;
			}

			// Low level
			(void) audio_cv_out.setVoltage(AudioCvOutChannel::kChannelA, 0.0f);
			(void) audio_cv_out.setVoltage(AudioCvOutChannel::kChannelB, 0.0f);
			ch = getchar_timeout_us(half_period_us);
			if (ch == '\n' || ch == '\r') {
				break;
			}
		}

		// After stopping, leave outputs at 0V for safety
		(void) audio_cv_out.setVoltage(AudioCvOutChannel::kChannelA, 0.0f);
		(void) audio_cv_out.setVoltage(AudioCvOutChannel::kChannelB, 0.0f);
	}

	// Return to DC coupling
	audio_cv_out.setCoupling(AudioCvOutChannel::kChannelA, AudioCvOutCoupling::kDcCoupled);
	audio_cv_out.setCoupling(AudioCvOutChannel::kChannelB, AudioCvOutCoupling::kDcCoupled);
	printf("Returned to DC coupling\n");
	printf("DC coupling: outputting ~60Hz square wave (0/10V). Press Enter to stop...\n");

	// Generate a simple ~60Hz square wave until user presses Enter
	{
		const uint32_t half_period_us = 1000000 / (60 * 2);	 // ~8.33 ms
		bool stop = false;
		while (!stop) {
			// High level
			(void) audio_cv_out.setVoltage(AudioCvOutChannel::kChannelA, 10.0f);
			(void) audio_cv_out.setVoltage(AudioCvOutChannel::kChannelB, 10.0f);
			int ch = getchar_timeout_us(half_period_us);
			if (ch == '\n' || ch == '\r') {
				break;
			}

			// Low level
			(void) audio_cv_out.setVoltage(AudioCvOutChannel::kChannelA, 0.0f);
			(void) audio_cv_out.setVoltage(AudioCvOutChannel::kChannelB, 0.0f);
			ch = getchar_timeout_us(half_period_us);
			if (ch == '\n' || ch == '\r') {
				break;
			}
		}

		// After stopping, leave outputs at 0V for safety
		(void) audio_cv_out.setVoltage(AudioCvOutChannel::kChannelA, 0.0f);
		(void) audio_cv_out.setVoltage(AudioCvOutChannel::kChannelB, 0.0f);
	}

	// Test edge cases
	printf("Testing edge cases...\n");

	// Test minimum voltage
	if (audio_cv_out.setVoltage(AudioCvOutChannel::kChannelA, 0.0f)) {
		printf("Set minimum voltage (0V) - OK\n");
	}
	waitForEnter();

	// Test maximum voltage
	if (audio_cv_out.setVoltage(AudioCvOutChannel::kChannelA, 10.0f)) {
		printf("Set maximum voltage (10V) - OK\n");
	}
	waitForEnter();

	// Test invalid voltage (should fail)
	if (!audio_cv_out.setVoltage(AudioCvOutChannel::kChannelA, 15.0f)) {
		printf("Rejected invalid voltage (15V) - OK\n");
	}
	waitForEnter("Press Enter to finish test...");

	printf("AudioCvOut test completed successfully!\n");
}
