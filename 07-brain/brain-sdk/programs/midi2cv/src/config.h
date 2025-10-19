// Configuration and calibration for midi2cv
// Defines MIDI channel filter and 1 V/oct calibration factors

#ifndef MIDI2CV_CONFIG_H
#define MIDI2CV_CONFIG_H

#include <cstdint>

namespace config {

// MIDI channel: 1-16. Set omni to true to accept all channels.
static constexpr uint8_t kMidiChannel = 1;
static constexpr bool kOmniMode = false;

// 1 V/oct mapping calibration: Vout = offset + scale * Vraw
// Where Vraw = (note - 12) / 12.0 (C0 = 0V)
static constexpr float kCalOffsetVolts = 0.0f;
static constexpr float kCalScale = 1.0f;  // match end-to-end 1 V/oct

}  // namespace config

#endif	// MIDI2CV_CONFIG_H
