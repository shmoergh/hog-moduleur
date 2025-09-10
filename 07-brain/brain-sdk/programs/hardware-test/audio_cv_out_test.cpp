#include <cstdio>

#include <pico/stdlib.h>

#include "brain-io/audio-cv-out.h"

using brain::io::AudioCvOut;
using brain::io::AudioCvOutChannel;
using brain::io::AudioCvOutCoupling;

void testAudioCvOut() {
	printf("AudioCvOut Test Starting...\n");
	
	// Initialize audio CV output with default pins
	static AudioCvOut audio_cv_out;
	if (!audio_cv_out.init()) {
		printf("ERROR: Failed to initialize AudioCvOut\n");
		return;
	}
	printf("AudioCvOut initialized successfully\n");
	
	// Test voltage sweep on both channels
	printf("Testing voltage sweep (0-10V)...\n");
	for (int i = 0; i <= 5; i++) {
		float voltage = static_cast<float>(i * 2);  // 0, 2, 4, 6, 8, 10V
		
		// Set voltage on both channels
		if (!audio_cv_out.setVoltage(AudioCvOutChannel::kChannelA, voltage)) {
			printf("ERROR: Failed to set voltage %.1fV on channel A\n", voltage);
		}
		if (!audio_cv_out.setVoltage(AudioCvOutChannel::kChannelB, voltage)) {
			printf("ERROR: Failed to set voltage %.1fV on channel B\n", voltage);
		}
		
		printf("Set both channels to %.1fV\n", voltage);
		sleep_ms(300);
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
	sleep_ms(500);
	
	// Test AC coupling  
	audio_cv_out.setCoupling(AudioCvOutChannel::kChannelA, AudioCvOutCoupling::kAcCoupled);
	audio_cv_out.setCoupling(AudioCvOutChannel::kChannelB, AudioCvOutCoupling::kAcCoupled);
	printf("Set AC coupling on both channels\n");
	sleep_ms(500);
	
	// Return to DC coupling
	audio_cv_out.setCoupling(AudioCvOutChannel::kChannelA, AudioCvOutCoupling::kDcCoupled);
	audio_cv_out.setCoupling(AudioCvOutChannel::kChannelB, AudioCvOutCoupling::kDcCoupled);
	printf("Returned to DC coupling\n");
	
	// Test edge cases
	printf("Testing edge cases...\n");
	
	// Test minimum voltage
	if (audio_cv_out.setVoltage(AudioCvOutChannel::kChannelA, 0.0f)) {
		printf("Set minimum voltage (0V) - OK\n");
	}
	
	// Test maximum voltage
	if (audio_cv_out.setVoltage(AudioCvOutChannel::kChannelA, 10.0f)) {
		printf("Set maximum voltage (10V) - OK\n");  
	}
	
	// Test invalid voltage (should fail)
	if (!audio_cv_out.setVoltage(AudioCvOutChannel::kChannelA, 15.0f)) {
		printf("Rejected invalid voltage (15V) - OK\n");
	}
	
	printf("AudioCvOut test completed successfully!\n");
}