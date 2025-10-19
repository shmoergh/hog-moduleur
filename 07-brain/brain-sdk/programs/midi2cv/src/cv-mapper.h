// CV mapping: MIDI note -> 1 V/oct volts with calibration

#ifndef MIDI2CV_CV_MAPPER_H
#define MIDI2CV_CV_MAPPER_H

#include <cstdint>

#include "config.h"

class CvMapper {
	public:
	// Convert MIDI note (0..127) to volts using 1 V/oct, C0 = 0 V
	// V = offset + scale * ((note - 12) / 12)
	float noteToVolts(uint8_t note) const {
		float vraw = (static_cast<int>(note) - 12) / 12.0f;
		float v = config::kCalOffsetVolts + config::kCalScale * vraw;
		printf("\r\n Raw voltage: %f", v);
		return v;
	}
};

#endif	// MIDI2CV_CV_MAPPER_H
