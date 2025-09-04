#include "midi_handler.h"

void MidiHandler::init() {
    m_input_buffer.init(m_buffer_var, MIDI_BUFFER_SIZE);
    m_gate = 0;
    m_cv = 0;
    m_note = -1;
    // m_settings_note_offs = 0;
    bend = PITCH_BEND_CENTER;
    pitch_bend_dirty = false;

    // Set all notes of the note stack to -1. This also indicates that there's
    // no incoming note (all notes released). The new notes always push all the
    // other notes in the stack to the right but only until [stack size - 1]
    // is reached. This way -1 never gets deleted from the stack so we always
    // know when all notes are released.
    for (int i = 0; i < NOTE_STACK_SIZE; i++) {
        note_stack[i] = -1;
    }

    uart_init(MIDI_UART_INSTANCE, MIDI_BAUDRATE);
    gpio_set_function(GP_MIDI_RX, GPIO_FUNC_UART);
}

void MidiHandler::read() {
    midi_event = -1;

    if (!uart_is_readable(MIDI_UART_INSTANCE)) return;

    uint8_t data = uart_getc(MIDI_UART_INSTANCE);
    m_input_buffer.write_byte(data);

    m_parse_midi();
}

void MidiHandler::note_on(uint8_t channel, uint8_t note, uint8_t velocity) {
    midi_event = NOTE_ON;

    // Do nothing if it's the same note that's already been playing
    if (m_note == note) {
        return;
    }

    m_push_note(note);
    m_note = note;
    note_playing = (note_stack[0] != -1);
}

void MidiHandler::note_off(uint8_t channel, uint8_t note, uint8_t velocity) {
    midi_event = NOTE_OFF;
    m_pop_note(note);
    m_note = note_stack[0];
    note_playing = (note_stack[0] != -1);
}

void MidiHandler::pitch_bend(uint8_t channel, uint16_t bend) {
    this->bend = bend;
    if (bend == m_last_pitch_bend) {
        pitch_bend_dirty = false;
    } else {
        pitch_bend_dirty = true;
        m_last_pitch_bend = bend;
    }
}

void MidiHandler::cc(uint8_t channel, uint8_t data1, uint8_t data2) {
    midi_event = CTRL_CHANGE;

    // Check for Modwheel CC (standard CC 1)
    if (data1 == 0x01) {
        printf("Modwheel: %d\r\n", data2);
        modwheel = data2;
    }
}

int MidiHandler::note() {
    return m_note + (12 * MIDI_OCTAVE_SHIFT);
}

/**
 * Read MIDI messages from buffer and call appropriate MIDI message. Messages
 * are parsed by the midi_parser library (parent class parse_byte method),
 * which eventually calls one of the redefined MIDI functions of this class.
 */
void MidiHandler::m_parse_midi() {
    while (!m_input_buffer.is_empty()) {
        uint8_t byte = 0;
        m_input_buffer.read_byte(byte);

        this->parse_byte(byte);  // Parent class call
    }
}

void MidiHandler::m_push_note(uint8_t note) {

    // Check if note exists in the stack. If it does, do nothing.
    if (m_find_note(note)) {
        return;
    }

    // Use two temp variables to push notes in the stack to the right. The last
    // note played is always in note_stack[0].
    int i = 0;
    int temp = note_stack[0], temp2;

    // We have to keep a 0 in the last stack position to be able to zero out
    // all notes when popping them out of the stack on note off events. That's
    // the reason we loop only until NOTE_STACK_SIZE - 1.
    while (temp && (i < NOTE_STACK_SIZE - 1)) {
         // Right shift stack
        temp2 = temp;
        temp = note_stack[i + 1];
        if (i < (NOTE_STACK_SIZE - 1)) {
            note_stack[i + 1] = temp2;
        }
        i++;
    }

    // Put the freshest note to the first position in the stack
    note_stack[0] = note;
}

void MidiHandler::m_pop_note(uint8_t note) {
    bool shift = false;

    // Find the released note and remove from the stack while pushing
    // all other notes to the left. The last element of the stack is
    // always 0.
    for (int i = 0; i < (NOTE_STACK_SIZE - 1); i++) {
        if (note_stack[i] == note) {
            shift = true;
        }
        if (shift) {
            note_stack[i] = note_stack[i + 1];
        }
    }
}

int MidiHandler::m_find_note(uint8_t note) {
    for (int i = 0; i < NOTE_STACK_SIZE; i++) {
        if (note_stack[i] == note) return i;
    }
    return 0;
}