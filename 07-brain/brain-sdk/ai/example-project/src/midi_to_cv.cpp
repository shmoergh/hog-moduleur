#include "midi_to_cv.h"

void MidiToCV::init() {
    m_cv = 0;
    m_gate = false;
    m_last_cv = 0;
    m_last_modwheel = 0;
    m_settings_note_counter = 0;
    m_settings_mode = false;
    m_reset_note_stack();

    gpio_init(GP_GATE);
    gpio_set_dir(GP_GATE, GPIO_OUT);
    gpio_pull_down(GP_GATE);
    gpio_put(GP_GATE, 0);
}

void MidiToCV::attach(MCP48X2 *dac) {
    m_dac = dac;
}

void MidiToCV::attach(UI *ui) {
    m_ui = ui;
}

void MidiToCV::attach(MidiHandler *midi_handler) {
    m_midi_handler = midi_handler;
}

void MidiToCV::attach(Settings *settings) {
    m_settings = settings;
}

/**
 * Main process. Put this in the main program loop for continuous midi to CV
 * conversion.
 */
void MidiToCV::process() {

    // Read MIDI messages. This builds a note stack in m_midi.note_stack
    m_midi_handler->read();

    // Go to settings mode by holding two notes, turn on the device and release the notes
    if (!m_settings_scan_finished) {
        if (m_midi_handler->midi_event == NOTE_OFF) {
            m_settings_note_counter++;
            if (m_settings_note_counter == 2) {
                m_settings_scan_finished = true;
                m_settings_mode = true;
                m_ui->toggle_blink();

                printf("Settings mode\r\n");
                m_settings_note_counter = 0;
            }
        }
        if (m_midi_handler->midi_event == NOTE_ON) {
            m_settings_scan_finished = true;
            m_settings_mode = false;
        } else {
            return;
        }
    } else {
        if (m_settings_mode) {
            m_process_settings();
            return;
        }
    }

    // If the MIDI channel is not the same as the one set in the settings then return
    if (m_midi_handler->channel() + 1 != m_settings->midi_channel) {
        return;
    }

    // If there's a note playing
    if (m_midi_handler->note_playing) {
        m_cv = get_note_cv(m_midi_handler->note());
        m_gate = true;
    } else {
        m_gate = false;
    }

    // Get pitch bend
    if (m_midi_handler->pitch_bend_dirty) {
        m_pitch_bend_cv = get_pitch_bend_cv(m_midi_handler->bend);
    }

    // Set CV voltage
    int cv = m_cv + m_pitch_bend_cv;
    cv = cv < 0 ? 0 : cv;

    if (cv != m_last_cv) {
        if (m_last_dac_channel != MCP48X2_CHANNEL_A) {
            m_dac->config(MCP48X2_CHANNEL_A, MCP48X2_GAIN_X2, 1);
            m_last_dac_channel = MCP48X2_CHANNEL_A;
        }
        m_dac->write(cv);
        m_last_cv = cv;
        printf("CV: %d\r\n", cv);
    }

    if (m_midi_handler->midi_event == CTRL_CHANGE) {
        if (m_midi_handler->modwheel != m_last_modwheel) {
            m_last_modwheel = m_midi_handler->modwheel;
            uint16_t raw_modwheel_cv = (uint16_t)Utils::map(m_last_modwheel, 0, 127, 0, MAX_MODWHEEL_VOLTAGE);
            if (m_last_dac_channel != MCP48X2_CHANNEL_B) {
                m_dac->config(MCP48X2_CHANNEL_B, MCP48X2_GAIN_X2, 1);
                m_last_dac_channel = MCP48X2_CHANNEL_B;
            }
            printf("Modwheel CV: %d\r\n", raw_modwheel_cv);
            m_dac->write(raw_modwheel_cv);
        }
    }

    // Set gate
    set_gate(m_gate);
}

uint16_t MidiToCV::get_note_cv(uint8_t note) {

    // Return calibrated CV value
    uint8_t t_note = note;
    uint8_t notes = OCTAVES * 12;
    if (t_note > LOWEST_MIDI_NOTE + notes) {
        t_note = LOWEST_MIDI_NOTE + notes;
    }
    uint8_t octave = floor(t_note / 12) + 1;
    uint16_t volt_per_octave = (uint16_t) (MAX_NOTE_VOLTAGE / OCTAVES);
    uint16_t raw_cv_lo = volt_per_octave * (octave - 1);
    uint16_t raw_cv_hi = volt_per_octave * octave;

    uint16_t raw_cv = (uint16_t)Utils::map(t_note, LOWEST_MIDI_NOTE, (LOWEST_MIDI_NOTE + notes), 0, MAX_NOTE_VOLTAGE);
    uint16_t cv = (uint16_t)Utils::map(raw_cv, raw_cv_lo, raw_cv_hi, (raw_cv_lo + m_settings->calibration[octave - 1]), (raw_cv_hi + m_settings->calibration[octave]));

    return cv;
}

/**
 * Pitch bend value can be between 0 and 0x3fff with 0x2000 meaning no bend.
 * Pitch bend CV calculation:
 * 1. Shift bend value to -0x2000 and 0x2000
 * 2. Get max bend CV value (2 semitones)
 * 3. Calculate actual bend CV value with the bend vs. max bend ratio
*/
int16_t MidiToCV::get_pitch_bend_cv(uint16_t bend) {
    int16_t shiftedBend = bend - PITCH_BEND_CENTER;
    uint8_t maxBendCV = (uint8_t) (MAX_NOTE_VOLTAGE / (OCTAVES * 12) * MAX_PB_SEMINOTES);
    return shiftedBend * maxBendCV / PITCH_BEND_CENTER; // BEND / MAX_BEND_VALUE[8192]] = BENDCV / MAX_BEND_CV
}

void MidiToCV::set_gate(bool on) {
    gpio_put(GP_GATE, on);
}

/**
 * Process settings mode. This is called when the user holds two notes and
 * releases after startup. The only setting now is the MIDI channel. The value
 * is set by pressing keys on the keyboard and it's set in binary according
 * to the following digits:
 *  C: digit 2^0
 *  D: digit 2^1
 *  E: digit 2^2
 *  F: digit 2^3
 *  G: digit 2^4
 *
 * So for channel 5 you have to press E and F then release. For channel 16 you
 * have to press G.
 */
void MidiToCV::m_process_settings() {
    // Save settings note stack if there's a note on event
    if (m_midi_handler->midi_event == NOTE_ON) {
        m_setting_note_stack[m_settings_note_counter] = m_midi_handler->note();
        m_settings_note_counter++;
    }

    // Read settings if there's a note off event
    if (m_midi_handler->midi_event == NOTE_OFF && m_settings_note_counter > 0) {
        int new_midi_channel = 0;

        for (int i = 0; i < NOTE_STACK_SIZE; i++) {
            if (m_setting_note_stack[i] != -1) {
                switch (m_setting_note_stack[i] % 12) {
                    // C note
                    case 0:
                        new_midi_channel += pow(2, 0);
                        break;

                    // D note
                    case 2:
                        new_midi_channel += pow(2, 1);
                        break;

                    // E note
                    case 4:
                        new_midi_channel += pow(2, 2);
                        break;

                    // F note
                    case 5:
                        new_midi_channel += pow(2, 3);
                        break;

                    // G note
                    case 7:
                        new_midi_channel += pow(2, 4);
                        break;
                }
            }
        }

        m_settings->set_midi_channel(new_midi_channel);

        // Blink LED fast for 1 second to indicate that the settings have been saved
        m_ui->fast_blink(1000);

        m_reset_note_stack();
        m_settings_note_counter = 0;
    }
}

void MidiToCV::m_reset_note_stack() {
    for (int i = 0; i < NOTE_STACK_SIZE; i++) {
        m_setting_note_stack[i] = -1;
    }
}