// MIDI → CV main program: maps MIDI notes to 1 V/oct CV on DAC A
// Dependencies: brain-io (MidiParser, AudioCvOut, Pulse), brain-common pins
// Hardware notes: MCP4822 on SPI; PULSE output is hardware-inverted (handled by Pulse)
// Owner: midi2cv app

#include <pico/stdlib.h>
#include <stdint.h>

#include <cstdio>
#include <optional>

#include "brain-common/brain-common.h"
#include "brain-io/audio-cv-out.h"
#include "brain-io/midi-parser.h"
#include "brain-io/pulse.h"
#include "config.h"
#include "cv-mapper.h"
#include "note-stack.h"

using brain::io::AudioCvOut;
using brain::io::AudioCvOutChannel;
using brain::io::MidiParser;
using brain::io::Pulse;

// Global singletons for callbacks (program scope)
static AudioCvOut g_dac;
static Pulse g_gate;  // uses default Brain PULSE pins
static NoteStack g_stack;
static CvMapper g_cv_mapper;

static inline void applyCurrentNoteCv();

// MIDI callbacks (C-style function pointers)
static void onNoteOn(uint8_t note, uint8_t velocity, uint8_t channel) {
	(void) velocity;
	(void) channel;
	g_stack.pushTop(note);
	g_gate.set(true);  // logical HIGH (Pulse handles hardware inversion)
	applyCurrentNoteCv();
}

static void onNoteOff(uint8_t note, uint8_t velocity, uint8_t channel) {
	(void) velocity;
	(void) channel;
    g_stack.remove(note);
    if (g_stack.isEmpty()) {
        // No active note -> gate LOW, hold last CV
        g_gate.set(false);
    } else {
        applyCurrentNoteCv();
    }
}

static inline void applyCurrentNoteCv() {
	auto top = g_stack.topOrNone();
	float volts = 0.0f;
	if (top.has_value()) {
		volts = g_cv_mapper.noteToVolts(top.value());
	}
	// Clamp to supported range [0, 10]
	if (volts < 0.0f) volts = 0.0f;
	if (volts > 10.0f) volts = 10.0f;

	printf("\r\n Volt: %f", volts);

	g_dac.setVoltage(AudioCvOutChannel::kChannelA, volts);
}

int main() {
	stdio_init_all();

	// Initialize DAC (SPI pins from Brain defaults). Force DC coupling and 0V on both.
	if (!g_dac.init()) {
		fprintf(stderr, "midi2cv: DAC init failed\n");
	}
	g_dac.setCoupling(AudioCvOutChannel::kChannelA, brain::io::AudioCvOutCoupling::kDcCoupled);
	g_dac.setCoupling(AudioCvOutChannel::kChannelB, brain::io::AudioCvOutCoupling::kDcCoupled);
	g_dac.setVoltage(AudioCvOutChannel::kChannelA, 0.0f);
	g_dac.setVoltage(AudioCvOutChannel::kChannelB, 0.0f);

	// Initialize Gate output (default Pulse pins).
	g_gate.begin();
	g_gate.set(false);	// LOW at startup

	// Configure MIDI parser
	MidiParser parser(config::kMidiChannel, config::kOmniMode);
	parser.setNoteOnCallback(&onNoteOn);
	parser.setNoteOffCallback(&onNoteOff);
	if (!parser.initUart()) {
		fprintf(stderr, "midi2cv: MIDI UART init failed\n");
	}

	printf("midi2cv: ready (ch=%u, omni=%d)\n", parser.channel(), parser.omni());

	// Main loop: process UART MIDI and dispatch via callbacks
	while (true) {
		parser.processUartInput();
		tight_loop_contents();
	}

	return 0;
}
