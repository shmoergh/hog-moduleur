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
static constexpr float kCalOffsetVolts = -0.03f;
static constexpr float kCalScale = 1.0125f;	 // base slope

// Per-octave calibration at C notes (C1..C10), in millivolts.
// Offset is added to the final voltage after base mapping.
// Example: if measured C2 = 1.988 V, set entry for C2 to +12 (adds +12 mV).
// Interpolation: notes between Ck..C(k+1) get a linearly interpolated offset.
static constexpr int kCalCMinOct = 1;  // C1
static constexpr int kCalCMaxOct = 10;	// C10
static constexpr int kCalCCount = kCalCMaxOct - kCalCMinOct + 1;  // 10 entries
// Index 0 -> C1, 1 -> C2, ..., 9 -> C10

static constexpr int16_t kCalCOffsetsMv[kCalCCount] = {
	8,	// C1
	0,	// C2
	0,	// C3
	0,	// C4
	0,	// C5
	0,	// C6
	0,	// C7
	0,	// C8
	0,	// C9
	0  // C10
};

// -10,  // C2
// -18,  // C3
// -19,  // C4
// -20,  // C5
// -20,	 // C6

}  // namespace config

#endif	// MIDI2CV_CONFIG_H
