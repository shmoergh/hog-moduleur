#include "brain-io/audio-cv-in.h"

#include <hardware/gpio.h>
#include <pico/stdlib.h>

#include <cstdio>

namespace brain::io {

using namespace brain::constants;

bool AudioCvIn::init() {
	// Initialize ADC hardware
	adc_init();

	// Configure GPIO pins for ADC use
	adc_gpio_init(GPIO_BRAIN_AUDIO_CV_IN_A);  // GPIO 27 -> ADC1
	adc_gpio_init(GPIO_BRAIN_AUDIO_CV_IN_B);  // GPIO 28 -> ADC2

	// Calculate conversion parameters from calibration constants
	calculateConversionParameters();

	// Take initial readings
	update();

	return true;
}

void AudioCvIn::update() {
	// Read channel A (GPIO 27 = ADC1)
	adc_select_input(1);
	channel_raw_[BRAIN_AUDIO_CV_IN_CHANNEL_A] = adc_read();

	// Read channel B (GPIO 28 = ADC2)
	adc_select_input(2);
	channel_raw_[BRAIN_AUDIO_CV_IN_CHANNEL_B] = adc_read();
}

uint16_t AudioCvIn::getRaw(int channel) const {
	if (channel == BRAIN_AUDIO_CV_IN_CHANNEL_A || channel == BRAIN_AUDIO_CV_IN_CHANNEL_B) {
		return channel_raw_[channel];
	}
	return 0;
}

uint16_t AudioCvIn::getRawChannelA() const {
	return channel_raw_[BRAIN_AUDIO_CV_IN_CHANNEL_A];
}

uint16_t AudioCvIn::getRawChannelB() const {
	return channel_raw_[BRAIN_AUDIO_CV_IN_CHANNEL_B];
}

float AudioCvIn::getVoltage(int channel) const {
	if (channel == BRAIN_AUDIO_CV_IN_CHANNEL_A || channel == BRAIN_AUDIO_CV_IN_CHANNEL_B) {
		return adcToVoltage(channel_raw_[channel]);
	}
	return 0.0f;
}

float AudioCvIn::getVoltageChannelA() const {
	return adcToVoltage(channel_raw_[BRAIN_AUDIO_CV_IN_CHANNEL_A]);
}

float AudioCvIn::getVoltageChannelB() const {
	return adcToVoltage(channel_raw_[BRAIN_AUDIO_CV_IN_CHANNEL_B]);
}

float AudioCvIn::adcToVoltage(uint16_t adc_value) const {
	// Convert ADC reading to voltage
	float adc_voltage = (static_cast<float>(adc_value) / kAdcMaxValue) * kAdcVoltageRef;
	
	// Apply calibration to get original signal voltage
	return (adc_voltage * voltage_scale_) + voltage_offset_;
}

void AudioCvIn::calculateConversionParameters() {
	// Calculate linear conversion from measured ADC voltages to original signal voltages
	// Two known points: (kAudioCvInVoltageAtMinus5V, kAudioCvInMinVoltage) 
	//                   (kAudioCvInVoltageAtPlus5V, kAudioCvInMaxVoltage)
	
	float voltage_span = kAudioCvInVoltageAtPlus5V - kAudioCvInVoltageAtMinus5V;
	float signal_span = kAudioCvInMaxVoltage - kAudioCvInMinVoltage;
	
	// Scale factor: change in output per unit change in input
	voltage_scale_ = signal_span / voltage_span;
	
	// Offset: output value when input is zero
	voltage_offset_ = kAudioCvInMinVoltage - (kAudioCvInVoltageAtMinus5V * voltage_scale_);
}

}  // namespace brain::io