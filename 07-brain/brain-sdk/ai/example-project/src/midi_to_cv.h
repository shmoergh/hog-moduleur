#ifndef _MIDI_TO_CV_H
#define _MIDI_TO_CV_H

/**
 * Converts MIDI note to CV and gate
 *
 * Pitch CV
 * --------
 * Pitch CV is generated through a DAC (MCP4822) which can output 0-4095mV. To
 * cover the 10V range the DAC output value is amplified with an op-amp (TL07x).
 *
 * Pitch bend value can be between 0 and 0x3fff with 0x2000 meaning no bend.
 * Pitch bend CV calculation:
 * 1. shift bend value to -0x2000 and 0x2000
 * 2. get max bend CV value (2 semitones)
 * 3. calculate actual bend CV value with the bend vs. max bend ratio
 *
 * Gate
 * ----
 * Gate is output on a standard GPIO pin on the PICO (can be set in settings.h)
 * which is amplified with a standard op-amp (e.g. TL07x) or a level shifter
 * (e.g. CD4504).
 */

#include "settings.h"
#include <mcp48x2.h>
#include "ui.h"
#include "midi_handler.h"
#include "math.h"
#include <utils.h>

class MidiToCV {
public:
    void init();
    void attach(MCP48X2 *dac);
    void attach(UI *ui);
    void attach(MidiHandler *midi_handler);
    void attach(Settings *settings);
    void process();

    uint16_t get_note_cv(uint8_t note);
    int16_t get_pitch_bend_cv(uint16_t bend);
    void set_gate(bool on);

private:
    bool m_gate;
    int m_last_cv;
    uint8_t m_last_modwheel;
    uint16_t m_cv;
    int16_t m_pitch_bend_cv;
    mcp48x2_channel m_last_dac_channel;

    // Settings variables
    int m_setting_note_stack[NOTE_STACK_SIZE];
    bool m_settings_scan_finished;
    bool m_settings_mode;
    uint8_t m_settings_note_counter;

    MCP48X2 *m_dac;
    UI *m_ui;
    MidiHandler *m_midi_handler;
    Settings *m_settings;

    void m_reset_note_stack();
    void m_process_settings();
};

#endif