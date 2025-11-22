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
#include "brain-ui/led.h"
#include "config.h"
#include "cv-mapper.h"
#include "note-stack.h"

using brain::io::AudioCvOut;
using brain::io::AudioCvOutChannel;
using brain::io::MidiParser;
using brain::io::Pulse;
using brain::ui::Led;

// Global singletons for callbacks (program scope)
static AudioCvOut g_dac;
static Pulse g_gate;  // uses default Brain PULSE pins
static NoteStack g_stack;
static CvMapper g_cv_mapper;
static Led g_led(BRAIN_LED_1);

constexpr uint LED_PINS[] = {
	BRAIN_LED_1, BRAIN_LED_2, BRAIN_LED_3, BRAIN_LED_4, BRAIN_LED_5, BRAIN_LED_6};
constexpr uint NUM_LEDS = 6;

static inline void applyCurrentNoteCv();

// Debug logging (disable for production to prevent MIDI event drops due to printf blocking)
// Set to 1 to see Note On/Off events (useful for debugging)
// Set to 0 for production (prevents printf blocking that can drop MIDI events)
#define DEBUG_MIDI_EVENTS 0

// MIDI callbacks (C-style function pointers)
static void onNoteOn(uint8_t note, uint8_t velocity, uint8_t channel) {
	(void) velocity;
	(void) channel;
#if DEBUG_MIDI_EVENTS
	printf("[NoteON ] note=%u vel=%u ch=%u\r\n", note, velocity, channel);
#endif
	g_stack.pushTop(note);
	g_gate.set(true);  // logical HIGH (Pulse handles hardware inversion)
	g_led.on();
	applyCurrentNoteCv();
}

static void onNoteOff(uint8_t note, uint8_t velocity, uint8_t channel) {
	(void) velocity;
	(void) channel;
#if DEBUG_MIDI_EVENTS
	printf("[NoteOFF] note=%u vel=%u ch=%u\r\n", note, velocity, channel);
#endif
	g_stack.remove(note);
	if (g_stack.isEmpty()) {
		// No active note -> gate LOW, hold last CV
		g_gate.set(false);
		g_led.off();
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

	// NOTE: printf() in callback hot path causes blocking that can drop MIDI events
	// Uncomment for debugging only, not for production use
	// printf("\r\n Volt: %f", volts);

	g_dac.setVoltage(AudioCvOutChannel::kChannelA, volts);
}

int main() {
	stdio_init_all();

	sleep_ms(1000);

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

	// Initialize all LEDs
	brain::ui::Led leds[NUM_LEDS] = {brain::ui::Led(LED_PINS[0]), brain::ui::Led(LED_PINS[1]),
		brain::ui::Led(LED_PINS[2]), brain::ui::Led(LED_PINS[3]), brain::ui::Led(LED_PINS[4]),
		brain::ui::Led(LED_PINS[5])};

	// Reset all LEDs to off
	printf("Resetting all LEDs to OFF\n");
	for (uint i = 0; i < NUM_LEDS; i++) {
		leds[i].init();
		leds[i].off();
	}
	sleep_ms(200);

	// Running lights effect to show successful startup
	printf("Running startup LED sequence\n");
	for (uint i = 0; i < NUM_LEDS; i++) {
		leds[i].on();
		sleep_ms(100);
		leds[i].off();
	}
	sleep_ms(200);

	// Flash all LEDs twice to confirm ready
	for (uint flash = 0; flash < 2; flash++) {
		for (uint i = 0; i < NUM_LEDS; i++) {
			leds[i].on();
		}
		sleep_ms(150);
		for (uint i = 0; i < NUM_LEDS; i++) {
			leds[i].off();
		}
		sleep_ms(150);
	}

	g_led.init();

	// Main loop: process UART MIDI and dispatch via callbacks
	while (true) {
		parser.processUartInput();
		tight_loop_contents();
	}

	return 0;
}
