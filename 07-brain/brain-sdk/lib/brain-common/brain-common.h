/**
 * @file brain_common.h
 * @brief Common definitions and constants for all Brain modules
 *
 * This header provides shared definitions used across multiple Brain modules,
 * including GPIO pin assignments, common constants, and utility macros.
 */

#pragma once

// GPIO pin assignments - centralized for easy reference and updates
#include <cstdint>

#include "brain-gpio-setup.h"

#define BRAIN_LED_1 GPIO_BRAIN_LED_1
#define BRAIN_LED_2 GPIO_BRAIN_LED_2
#define BRAIN_LED_3 GPIO_BRAIN_LED_3
#define BRAIN_LED_4 GPIO_BRAIN_LED_4
#define BRAIN_LED_5 GPIO_BRAIN_LED_5
#define BRAIN_LED_6 GPIO_BRAIN_LED_6

#define BRAIN_BUTTON_1 GPIO_BRAIN_BUTTON_1
#define BRAIN_BUTTON_2 GPIO_BRAIN_BUTTON_2

// Common constants that might be used across modules
namespace brain {
namespace constants {
// Timing constants
constexpr uint32_t kMicrosPerSecond = 1000000;
constexpr uint32_t kMillisPerSecond = 1000;

// ADC constants
constexpr uint16_t kAdcMaxValue = 4095;	 // 12-bit ADC
constexpr float kAdcVoltageRef = 3.3f;	// Pico ADC reference voltage

// Default sample rates
constexpr uint32_t kDefaultAudioSampleRate = 44100;
constexpr uint32_t kDefaultControlRate = 1000;

// Audio CV In calibration constants (update after hardware testing)
constexpr float kAudioCvInVoltageAtMinus5V = 0.240f;  // Approx 240mV for -5V input
constexpr float kAudioCvInVoltageAtPlus5V = 3.000f;   // Approx 3V for +5V input

// Original signal voltage range
constexpr float kAudioCvInMinVoltage = -5.0f;
constexpr float kAudioCvInMaxVoltage = 5.0f;
}  // namespace constants
}  // namespace brain

// Audio CV In channel constants
#define BRAIN_AUDIO_CV_IN_CHANNEL_A 0
#define BRAIN_AUDIO_CV_IN_CHANNEL_B 1
