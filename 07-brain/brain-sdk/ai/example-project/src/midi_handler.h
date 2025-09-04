/**
 * @file midi_handler.h
 * @author Peter Zimon (peterzimon.com)
 * @brief
 * Stores incoming MIDI notes in a ring buffer and returns it as the notes are
 * pushed or released.
 *
 * @version 0.1
 * @date 2022-06-03
 *
 * @copyright Copyright (c) 2022
 */

#ifndef _MIDI_HANDLER_H
#define _MIDI_HANDLER_H

#include <iostream>
#include "settings.h"
#include "hardware/uart.h"
#include <midi_parser.h>
#include <ringbuffer.h>

#define MIDI_BUFFER_SIZE 32
#define NOTE_STACK_SIZE 25

class MidiHandler: public MidiParser {
public:
    bool pitch_bend_dirty;
    bool note_playing;
    uint8_t modwheel;
    int midi_event;
    int note_stack[NOTE_STACK_SIZE];
    uint16_t bend;

    void init(void);
    void read();
    void note_on(uint8_t channel, uint8_t note, uint8_t velocity);
    void note_off(uint8_t channel, uint8_t note, uint8_t velocity);
    void pitch_bend(uint8_t channel, uint16_t bend);
    void cc(uint8_t channel, uint8_t data1, uint8_t data2);
    int note();

private:
    int m_gate;
    uint8_t m_buffer_var[MIDI_BUFFER_SIZE];
    int m_note;
    uint16_t m_cv;
    uint16_t m_last_pitch_bend;
    RingBuffer m_input_buffer;

    void m_parse_midi();
    void m_push_note(uint8_t note);
    void m_pop_note(uint8_t note);
    int m_find_note(uint8_t note);
};

#endif