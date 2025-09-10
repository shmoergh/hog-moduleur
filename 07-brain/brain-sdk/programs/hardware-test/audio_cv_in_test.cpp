#include <cstdio>

#include <pico/stdlib.h>

#include "brain-io/audio-cv-in.h"

using brain::io::AudioCvIn;

void testAudioCvIn() {
	printf("AudioCvIn Test Starting...\n");
	
	// Initialize audio CV input
	static AudioCvIn audio_cv_in;
	if (!audio_cv_in.init()) {
		printf("ERROR: Failed to initialize AudioCvIn\n");
		return;
	}
	printf("AudioCvIn initialized successfully\n");
	
	// Test continuous reading for several iterations
	printf("Reading both channels for 10 iterations...\n");
	printf("Format: [Iteration] Ch A: raw=XXXX voltage=X.XXV | Ch B: raw=XXXX voltage=X.XXV\n");
	
	for (int i = 1; i <= 10; i++) {
		// Update readings
		audio_cv_in.update();
		
		// Get raw ADC values (0-4095)
		uint16_t rawA = audio_cv_in.getRawChannelA();
		uint16_t rawB = audio_cv_in.getRawChannelB();
		
		// Get converted voltages (-5V to +5V)
		float voltageA = audio_cv_in.getVoltageChannelA();
		float voltageB = audio_cv_in.getVoltageChannelB();
		
		printf("[%2d] Ch A: raw=%4d voltage=%+6.3fV | Ch B: raw=%4d voltage=%+6.3fV\n",
			i, rawA, voltageA, rawB, voltageB);
		
		sleep_ms(200);  // 200ms between readings
	}
	
	// Test using channel constants
	printf("\nTesting channel access using constants...\n");
	audio_cv_in.update();
	
	uint16_t rawConstA = audio_cv_in.getRaw(BRAIN_AUDIO_CV_IN_CHANNEL_A);
	uint16_t rawConstB = audio_cv_in.getRaw(BRAIN_AUDIO_CV_IN_CHANNEL_B);
	float voltageConstA = audio_cv_in.getVoltage(BRAIN_AUDIO_CV_IN_CHANNEL_A);
	float voltageConstB = audio_cv_in.getVoltage(BRAIN_AUDIO_CV_IN_CHANNEL_B);
	
	printf("Using constants - Ch A: raw=%d voltage=%+6.3fV | Ch B: raw=%d voltage=%+6.3fV\n",
		rawConstA, voltageConstA, rawConstB, voltageConstB);
	
	// Test edge case - invalid channel
	printf("\nTesting invalid channel access...\n");
	uint16_t invalidRaw = audio_cv_in.getRaw(99);  // Invalid channel
	float invalidVoltage = audio_cv_in.getVoltage(99);
	printf("Invalid channel returns: raw=%d voltage=%f (should be 0)\n", invalidRaw, invalidVoltage);
	
	// Show calibration info
	printf("\nCalibration info (from brain-common.h constants):\n");
	printf("- ADC voltage at -5V input: %.3fV\n", brain::constants::kAudioCvInVoltageAtMinus5V);
	printf("- ADC voltage at +5V input: %.3fV\n", brain::constants::kAudioCvInVoltageAtPlus5V);
	printf("- Signal range: %.1fV to %.1fV\n", 
		brain::constants::kAudioCvInMinVoltage, brain::constants::kAudioCvInMaxVoltage);
	
	// Final continuous reading demonstration
	printf("\nContinuous reading demo (5 seconds)...\n");
	uint32_t start_time = time_us_32();
	uint32_t last_print = 0;
	
	while ((time_us_32() - start_time) < 5000000) {  // 5 seconds
		audio_cv_in.update();
		
		// Print every 500ms
		if ((time_us_32() - last_print) >= 500000) {
			float voltA = audio_cv_in.getVoltageChannelA();
			float voltB = audio_cv_in.getVoltageChannelB();
			printf("Live: A=%+6.3fV B=%+6.3fV\n", voltA, voltB);
			last_print = time_us_32();
		}
		
		sleep_ms(10);  // Small delay to avoid overwhelming the system
	}
	
	printf("AudioCvIn test completed successfully!\n");
}