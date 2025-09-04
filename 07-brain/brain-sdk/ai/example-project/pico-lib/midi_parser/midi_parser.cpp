#include "midi_parser.h"

/**
 * Parses incoming and constructs a midi message. As soon as the message is
 * complete, it calls m_send_message()
 */
void MidiParser::parse_byte(uint8_t byte) {

    // Received status message
    if (byte >= 0x80) {

        // Reset tracking variables
        m_received_data_bytes = 0;
        m_expected_data_size = 1;  // Default data size
        m_running_status = byte;
        uint8_t hi = byte & 0xf0;
        uint8_t lo = byte & 0x0f;

        // Set expected data size
        switch (hi)
        {
        case NOTE_OFF:
        case NOTE_ON:
        case POLY_AFTERTOUCH:
        case CTRL_CHANGE:
        case PITCH_BEND:
            m_expected_data_size = 2;
            break;

        case SYSEX:
            if (lo > 0 && lo < 3) {
                m_expected_data_size = 2;
            } else if (lo >= 4) {
                m_expected_data_size = 0;
            }
            break;

        case PROG_CHANGE:
        case CH_AFTERTOUCH:
            break;
        }

    // Received channel data
    } else {
        
        m_data[m_received_data_bytes] = byte;
        m_received_data_bytes++;

        if (m_received_data_bytes >= m_expected_data_size) {
            m_send_message();
            m_received_data_bytes = 0;      // Resetting received data
        }
    }
}

uint8_t MidiParser::channel() {
    return m_midi_channel;
}

/**
 * Calls the appropriate message method based on statuses. The methods are supposed
 * to be implemented in a child class of this class.
 * */
void MidiParser::m_send_message() {

    uint8_t hi = m_running_status & 0xf0;
    uint8_t lo = m_running_status & 0x0f;     // Lower part of the byte contains channel 
                                            // information for most regular MIDI messages

    switch (hi)
    {
    case NOTE_OFF:
        m_midi_channel = lo;
        note_off(lo, m_data[0], m_data[1]);
        break;

    case NOTE_ON:
        m_midi_channel = lo;
        if (m_data[1]) {
            note_on(lo, m_data[0], m_data[1]);
        } else {
            note_off(lo, m_data[0], m_data[1]);    // If velocity is zero, treat it as note off message
        }
        break;
    
    case POLY_AFTERTOUCH:
        aftertouch(lo, m_data[0], m_data[1]);
        break;

    case CTRL_CHANGE:
        cc(lo, m_data[0], m_data[1]);
        break;
    
    case PROG_CHANGE:
        program_change(lo, m_data[0]);
        break;

    case CH_AFTERTOUCH:
        aftertouch(lo, m_data[0]);
        break;

    case PITCH_BEND:
        pitch_bend(lo, ((uint16_t)m_data[1] << 7) + m_data[0]);
        break;
    
    // TBD
    case SYSEX:
        // NOP
        break;
    }
}