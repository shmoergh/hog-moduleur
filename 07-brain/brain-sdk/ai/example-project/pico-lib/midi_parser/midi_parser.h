/**
 * MidiParser
 * 
 * @author Peter Zimon (peter.zimon@gmail.com)
 * @copyright Copyright (c) 2022
 * 
 * Parses incoming bytes to midi messages. If a MIDI message is constructed
 * (complete) then it calls the corresponding method which can be implemented
 * by an inherited class.
 * 
 * How to use?
 * 1. Create an instance of the MidiParser class that will handle midi messages
 *    e.g. class MidiHandler: public MidiParser { }
 * 2. Call the parse_byte function in a loop to parse bytes coming from UART/USB
 * 3. Implement midi message methods (e.g. noteOn, noteOff etc.) in MidiHandler
 * */

#ifndef _PICO_LIB_MIDI_PARSER_H
#define _PICO_LIB_MIDI_PARSER_H

// High byte of status messages
#define NOTE_OFF            0x80
#define NOTE_ON             0x90
#define POLY_AFTERTOUCH     0xa0
#define CTRL_CHANGE         0xb0
#define PROG_CHANGE         0xc0
#define CH_AFTERTOUCH       0xd0
#define PITCH_BEND          0xe0
#define SYSEX               0xf0

#include <pico/stdlib.h>

class MidiParser {

    public:
        void parse_byte(uint8_t byte);
        uint8_t channel();

        // MIDI messages must be implemented in handler (child) class
        virtual void note_off(uint8_t channel, uint8_t note, uint8_t velocity) { }
        virtual void note_on(uint8_t channel, uint8_t note, uint8_t velocity) { }
        virtual void aftertouch(uint8_t channel, uint8_t note, uint8_t pressure) { }
        virtual void cc(uint8_t channel, uint8_t data1, uint8_t data2) { }
        virtual void program_change(uint8_t channel, uint8_t program) { }
        virtual void aftertouch(uint8_t channel, uint8_t pressure) { }
        virtual void pitch_bend(uint8_t channel, uint16_t bend) { }

    private:
        void m_send_message();

        uint8_t m_running_status;
        uint8_t m_data[3];
        uint8_t m_received_data_bytes;
        uint8_t m_expected_data_size;

        uint8_t m_midi_channel;
};

#endif
