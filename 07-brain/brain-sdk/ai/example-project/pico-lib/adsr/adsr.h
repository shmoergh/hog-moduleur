/**
 * ADSR class
 *
 * Portions of this code are derived from or inspired by the ADSR library
 * developed by mo-thunderz. https://github.com/mo-thunderz/adsr
 * The original code is licensed under the MIT License.
 * Details of the license can be found in the accompanying LICENSE file at https://github.com/mo-thunderz/adsr/blob/main/LICENSE.
 *
 * How does triggering work?
 *
 * The note_on() acts like a trigger, NOT like a gate. This means that it has to
 * called only once when an ADSR should be started. Similarly, note_off() should
 * be called once when the ADSR should start the release phase. So if you have a
 * gate like controller then you have to implement this logic. The is_on()
 * method helps in this.
 *
 * A simple implementation can look like this:
 *
 * ```
 * ADSR m_adsr(4096);
 * bool gate_on;
 *
 * if (gate_on && !m_adsr.is_on()) {
 *      m_adsr.note_on();
 * } else if (gate_off && m_adsr.is_on()) {
 *      m_adsr.note_off();
 * }
 * ```
 *
 * The reason for this is to make the class flexible, ie. for being able to re-
 * trigger the ADSR freely.
 * */

#ifndef _PICO_LIB_ADSR_H
#define _PICO_LIB_ADSR_H

// Definitions
#define LUT_SIZE 4096                               // Number of time samples for the lookup tables. It's
                                                    // essentially the resolution of the ADSR
#define DEFAULT_ATTACK_ALPHA 0.995                  // Varies between 0.9 (steep curve) and 0.9995 (straight line)
#define DEFAULT_DECAY_RELEASE 0.99                 // Fits to ARRAY_SIZE 1024
#define DEFAULT_ADR_uS 300000                       // Default Attack, Decay and Release in us
#define DEFAULT_SUSTAIN_LEVEL 0.5                   // Relative to max sustain

#include <pico/stdlib.h>

class ADSR {
public:

    // Constructor
    // Max value of the DAC = 2^resolution. E.g. for a 12bit DAC -> 4096
    ADSR(
        int dac_size,
        float attack_alpha = DEFAULT_ATTACK_ALPHA,
        float attack_decay_release = DEFAULT_DECAY_RELEASE
    );

    // ADSR value setters
    void set_attack(unsigned long l_attack);     // 0 to 20 sec in us
    void set_decay(unsigned long l_decay);       // 1ms to 60 sec in us
    void set_sustain(int l_sustain);             // 0 to DACSIZE-1
    void set_release(unsigned long l_release);   // 1ms to 60 sec in us

    // Gate
    void note_on();
    void note_off();

    // Options
    void set_reset_attack(bool l_reset_attack);   // if _reset_attack is true a new trigger starts with 0,
                                                // if _reset_attack is false it starts with the current output value

    bool is_on();

    // Output
    int envelope();

private:
    int _attack_table[LUT_SIZE];
    int _decay_release_table[LUT_SIZE];

    float _attach_alpha;
    float _attack_decay_release;

    int _vertical_resolution;                   // number of bits for output, control, etc
    uint64_t _attack = 0;
    uint64_t _decay = 0;
    int _sustain = 0;
    uint64_t _release = 0;
    bool _reset_attack = false;

    // Time stamp for note on and note off
    uint64_t _t_note_on = 0;
    uint64_t _t_note_off = 0;

    // Internal values needed to transition to new pulse (attack) and to release at any point in time
    int _adsr_output;
    int _release_start;
    int _attack_start;
    int _notes_pressed = 0;

    static inline long _map(long x, long in_min, long in_max, long out_min, long out_max) {
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }

    static inline uint64_t _micros() {
        return to_us_since_boot(get_absolute_time());
    }
};

#endif
