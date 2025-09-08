/**
 * @file brain_common.h
 * @brief Common definitions and constants for all Brain modules
 *
 * This header provides shared definitions used across multiple Brain modules,
 * including GPIO pin assignments, common constants, and utility macros.
 */

#pragma once

// GPIO pin assignments - centralized for easy reference and updates
#include "brain-gpio-setup.h"

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
}  // namespace constants
}  // namespace brain
