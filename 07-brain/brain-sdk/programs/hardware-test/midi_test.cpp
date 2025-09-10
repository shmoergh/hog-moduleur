#include <hardware/uart.h>
#include <pico/stdlib.h>

#include <cstdio>

#include "brain-io/midi-parser.h"
#include "settings.h"

brain::io::MidiParser midi_parser;

// Test completion tracking
struct TestState {
	bool note_on_received = false;
	bool note_off_received = false;
	bool pitch_bend_received = false;
	bool test_complete = false;
} test_state;

void note_on_cb(uint8_t note, uint8_t velocity, uint8_t channel) {
	printf("+ Note On received: Note=%u, Velocity=%u, Channel=%u\r\n", note, velocity, channel);
	test_state.note_on_received = true;
}

void note_off_cb(uint8_t note, uint8_t velocity, uint8_t channel) {
	printf("+ Note Off received: Note=%u, Velocity=%u, Channel=%u\r\n", note, velocity, channel);
	test_state.note_off_received = true;
}

void pitch_bend_cb(int16_t value, uint8_t channel) {
	printf("+ Pitch Bend received: Value=%d, Channel=%u\r\n", value, channel);
	test_state.pitch_bend_received = true;
}

void control_change_cb(uint8_t cc, uint8_t value, uint8_t channel) {
	printf("• Control Change received: CC=%u, Value=%u, Channel=%u\r\n", cc, value, channel);
	// CC messages are not required for test completion but we'll log them
}

void realtime_cb(uint8_t status) {
	printf("• Real-time message received: 0x%02X\r\n", status);
	// Real-time messages are not required for test completion but we'll log them
}

bool isTestComplete() {
	return test_state.note_on_received && test_state.note_off_received &&
		test_state.pitch_bend_received;
}

void waitForUserInput() {
	printf("\r\n=== MIDI INPUT TEST SETUP ===\r\n");
	printf("1. Connect a MIDI controller to the Brain module's MIDI input\r\n");
	printf("2. Ensure the controller is powered on and configured to send on MIDI channel 1\r\n");
	printf("3. Press Enter when ready to start the test...\r\n");

	// Wait for user to press Enter
	while (true) {
		int c = getchar_timeout_us(1000);  // 1ms timeout
		if (c == '\r' || c == '\n') {
			break;
		}
	}
}

void testMidi() {
	printf("MIDI Parser tests\n");
	printf("============================\n\n");

	// Wait for user to connect MIDI controller
	waitForUserInput();

	printf("\r\n=== INITIALIZING MIDI INPUT ===\r\n");

	// Initialize MIDI parser with default Brain module configuration
	if (!midi_parser.initUart()) {
		printf("ERROR: Failed to initialize MIDI UART!\r\n");
		return;
	}

	// Configure MIDI parser for channel 1 (not omni mode)
	midi_parser.setChannel(MIDI_CHANNEL);
	midi_parser.setOmni(false);

	// Set up callbacks
	midi_parser.setNoteOnCallback(note_on_cb);
	midi_parser.setNoteOffCallback(note_off_cb);
	midi_parser.setPitchBendCallback(pitch_bend_cb);
	midi_parser.setControlChangeCallback(control_change_cb);
	midi_parser.setRealtimeCallback(realtime_cb);

	printf(
		"MIDI input initialized using default Brain module configuration (GPIO 9, UART1, 31250 "
		"baud)\r\n");
	printf("Parser configured for MIDI Channel 1\r\n\r\n");

	printf("=== MIDI MESSAGE TEST ===\r\n");
	printf("Please send the following MIDI messages from your controller:\r\n");
	printf("1. Play a note (Note On + Note Off)\r\n");
	printf("2. Use pitch bend wheel or lever\r\n");
	printf("   (Control Change messages will also be logged but are not required)\r\n\r\n");

	printf("Test Progress:\r\n");
	printf("  Note On:     [ ] Waiting...\r\n");
	printf("  Note Off:    [ ] Waiting...\r\n");
	printf("  Pitch Bend:  [ ] Waiting...\r\n\r\n");

	// Test loop - process MIDI input until all required messages are received
	uint32_t last_status_time = 0;
	const uint32_t STATUS_UPDATE_INTERVAL_MS = 1000;  // Update status every second

	while (!isTestComplete()) {
		// Process any incoming MIDI data using the integrated UART processing
		midi_parser.processUartInput();

		// Update status display periodically
		uint32_t now = time_us_32() / 1000;	 // Convert to milliseconds
		if (now - last_status_time >= STATUS_UPDATE_INTERVAL_MS) {
			printf("\rStatus: Note On:[%c] Note Off:[%c] Pitch Bend:[%c] | Listening for MIDI...",
				test_state.note_on_received ? 'X' : ' ', test_state.note_off_received ? 'X' : ' ',
				test_state.pitch_bend_received ? 'X' : ' ');
			last_status_time = now;
		}

		sleep_ms(10);  // Small delay to prevent busy waiting
	}

	printf("\r\n\r\nSUCCESS: All required MIDI messages received!\r\n");
	printf("MIDI Parser test complete.\r\n");
	printf("-----\r\n");
}
