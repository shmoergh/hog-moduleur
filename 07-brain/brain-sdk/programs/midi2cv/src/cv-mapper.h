// CV mapping: MIDI note -> 1 V/oct volts with calibration

#ifndef MIDI2CV_CV_MAPPER_H
#define MIDI2CV_CV_MAPPER_H

#include <stdint.h>

#include <algorithm>

#include "config.h"

class CvMapper {
	public:
	// Convert MIDI note (0..127) to volts using 1 V/oct, C0 = 0 V
	// V = offset + scale * ((note - 12) / 12) + per-C interpolated mV offset
	float noteToVolts(uint8_t note) const {
		float vraw = (static_cast<int>(note) - 12) / 12.0f;
		float v = config::kCalOffsetVolts + config::kCalScale * vraw;
		// Add interpolated per-C offset (mV → V)
		float mv = static_cast<float>(offsetMvForNote(note));
		v += mv * 0.001f;
		return v;
	}

	private:
	static int16_t offsetMvForNote(uint8_t note) {
		// Lower C octave for note (C0=12)
		int m = (static_cast<int>(note) - 12) / 12;	 // may be < 0

		const int first_idx = 0;  // C1
		const int last_idx = config::kCalCCount - 1;  // C10

		// Clamp lower octave to [C1..C9] so we have a pair [lower, upper]
		int lower_oct = std::clamp(m, config::kCalCMinOct, config::kCalCMaxOct - 1);
		int lower_idx = lower_oct - config::kCalCMinOct;  // 0..8
		int upper_idx = std::min(lower_idx + 1, last_idx);	// 1..9

		// MIDI note number for the lower C anchor
		int lower_c_note = 12 + 12 * lower_oct;

		// Position within the octave
		float r = (static_cast<int>(note) - lower_c_note) / 12.0f;
		if (upper_idx == lower_idx) {
			r = 0.0f;  // at/above top or below bottom
		} else {
			if (r < 0.0f) r = 0.0f;
			if (r > 1.0f) r = 1.0f;
		}

		float lo = static_cast<float>(config::kCalCOffsetsMv[lower_idx]);
		float hi = static_cast<float>(config::kCalCOffsetsMv[upper_idx]);
		float mv = lo + (hi - lo) * r;
		return static_cast<int16_t>(mv);
	}
};

#endif	// MIDI2CV_CV_MAPPER_H
