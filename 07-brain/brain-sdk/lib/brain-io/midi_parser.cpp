#include "brain-io/midi_parser.h"

namespace brain::io {

MidiParser::MidiParser(uint8_t channel, bool omni) : channel_filter_(channel), omni_mode_(omni) {
	setChannel(channel);  // Clamp to valid range
	reset();
}

void MidiParser::reset() {
	state_ = State::Idle;
	running_status_ = 0;
	current_status_ = 0;
	data_[0] = 0;
	data_[1] = 0;
	data_count_ = 0;
	expected_data_bytes_ = 0;
}

void MidiParser::setChannel(uint8_t ch) {
	// Clamp to valid MIDI channel range (1-16)
	if (ch < 1) {
		channel_filter_ = 1;
	} else if (ch > 16) {
		channel_filter_ = 16;
	} else {
		channel_filter_ = ch;
	}
}

uint8_t MidiParser::channel() const {
	return channel_filter_;
}

void MidiParser::setOmni(bool enabled) {
	omni_mode_ = enabled;
}

bool MidiParser::omni() const {
	return omni_mode_;
}

void MidiParser::feed(uint8_t byte) noexcept {
	// Handle real-time messages immediately at any time
	if (isRealtimeByte(byte)) {
		handleRealtimeByte(byte);
		return;
	}

	// Ignore System Common messages (SysEx, etc.) for v1
	if (isSystemCommonByte(byte)) {
		reset();  // Clear any partial message
		return;
	}

	if (isStatusByte(byte)) {
		// New status byte received
		current_status_ = byte;
		running_status_ = byte;	 // Update running status
		expected_data_bytes_ = getExpectedDataBytes(byte);

		if (expected_data_bytes_ == 0) {
			// No data bytes expected, process immediately
			processMessage();
			state_ = State::Idle;
		} else if (expected_data_bytes_ == 1) {
			state_ = State::AwaitData1;
			data_count_ = 0;
		} else {
			state_ = State::AwaitData1;
			data_count_ = 0;
		}
	} else if (isDataByte(byte)) {
		// Data byte received
		switch (state_) {
			case State::Idle:
				// Use running status if available
				if (running_status_ != 0) {
					current_status_ = running_status_;
					expected_data_bytes_ = getExpectedDataBytes(current_status_);
					data_[0] = byte;
					data_count_ = 1;

					if (expected_data_bytes_ == 1) {
						processMessage();
						state_ = State::Idle;
					} else {
						state_ = State::AwaitData2;
					}
				}
				break;

			case State::AwaitData1:
				data_[0] = byte;
				data_count_ = 1;

				if (expected_data_bytes_ == 1) {
					processMessage();
					state_ = State::Idle;
				} else {
					state_ = State::AwaitData2;
				}
				break;

			case State::AwaitData2:
				data_[1] = byte;
				data_count_ = 2;
				processMessage();
				state_ = State::Idle;
				break;
		}
	} else {
		// Invalid byte, reset state
		reset();
	}
}

void MidiParser::setNoteOnCallback(NoteOnCallback callback) {
	note_on_callback_ = callback;
}

void MidiParser::setNoteOffCallback(NoteOffCallback callback) {
	note_off_callback_ = callback;
}

void MidiParser::setControlChangeCallback(ControlChangeCallback callback) {
	control_change_callback_ = callback;
}

void MidiParser::setPitchBendCallback(PitchBendCallback callback) {
	pitch_bend_callback_ = callback;
}

void MidiParser::setRealtimeCallback(RealtimeCallback callback) {
	realtime_callback_ = callback;
}

bool MidiParser::shouldProcessChannel(uint8_t messageChannel) const {
	if (omni_mode_) {
		return true;
	}
	// messageChannel is 0-15, channel_filter_ is 1-16
	return (messageChannel + 1) == channel_filter_;
}

void MidiParser::processMessage() {
	uint8_t status_type = getStatusType(current_status_);
	uint8_t message_channel = getStatusChannel(current_status_);

	// Check channel filter
	if (!shouldProcessChannel(message_channel)) {
		return;
	}

	// Convert channel from 0-15 to 1-16 for callbacks
	uint8_t callback_channel = message_channel + 1;

	switch (status_type) {
		case kNoteOnMask:
			if (data_count_ >= 2) {
				uint8_t note = data_[0];
				uint8_t velocity = data_[1];

				// Treat Note On with velocity 0 as Note Off per MIDI spec
				if (velocity == 0) {
					if (note_off_callback_) {
						note_off_callback_(note, velocity, callback_channel);
					}
				} else {
					if (note_on_callback_) {
						note_on_callback_(note, velocity, callback_channel);
					}
				}
			}
			break;

		case kNoteOffMask:
			if (data_count_ >= 2 && note_off_callback_) {
				uint8_t note = data_[0];
				uint8_t velocity = data_[1];
				note_off_callback_(note, velocity, callback_channel);
			}
			break;

		case kControlChangeMask:
			if (data_count_ >= 2 && control_change_callback_) {
				uint8_t cc = data_[0];
				uint8_t value = data_[1];
				control_change_callback_(cc, value, callback_channel);
			}
			break;

		case kPitchBendMask:
			if (data_count_ >= 2 && pitch_bend_callback_) {
				uint8_t lsb = data_[0];
				uint8_t msb = data_[1];

				// Combine to 14-bit value (0..16383)
				uint16_t bend_value = (static_cast<uint16_t>(msb) << 7) | lsb;

				// Convert to signed range (-8192..+8191)
				int16_t signed_bend = static_cast<int16_t>(bend_value) - 8192;

				pitch_bend_callback_(signed_bend, callback_channel);
			}
			break;

		default:
			// Unknown or unsupported message type
			break;
	}
}

void MidiParser::handleRealtimeByte(uint8_t byte) {
	if (realtime_callback_) {
		realtime_callback_(byte);
	}
}

uint8_t MidiParser::getExpectedDataBytes(uint8_t status) const {
	uint8_t status_type = getStatusType(status);

	switch (status_type) {
		case kNoteOnMask:
		case kNoteOffMask:
		case kControlChangeMask:
		case kPitchBendMask:
			return 2;

		default:
			return 0;
	}
}

}  // namespace brain::io
