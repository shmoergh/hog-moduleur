#include "adsr.h"
#include <math.h>

ADSR::ADSR(
    int l_vertical_resolution, 
    float attack_alpha,
    float attack_decay_release
)
{
    // Initialise
    _vertical_resolution = l_vertical_resolution;

    _attach_alpha = attack_alpha;
    _attack_decay_release = attack_decay_release;

    _attack = DEFAULT_ADR_uS;
    _decay = DEFAULT_ADR_uS;
    _sustain = l_vertical_resolution * DEFAULT_SUSTAIN_LEVEL;                   
    _release = DEFAULT_ADR_uS;

    // Create look-up table for Attack
    for (int i = 0; i < LUT_SIZE; i++) {
        _attack_table[i] = i;
        _decay_release_table[i] = _vertical_resolution - 1 - i;
    }

      // Create look-up table for Decay and Release
    for (int i = 0; i < LUT_SIZE - 1; i++) {
        _attack_table[i+1] = (1.0 - _attach_alpha) * (_vertical_resolution - 1) + _attach_alpha * _attack_table[i];
        _decay_release_table[i+1] = _attack_decay_release * _decay_release_table[i];
    }

    // Normalize tables to min and max
    for (int i = 0; i < LUT_SIZE; i++) {
        _attack_table[i] = _map(
            _attack_table[i], 
            0, 
            _attack_table[LUT_SIZE - 1], 
            0, 
            _vertical_resolution - 1
        );
        
        _decay_release_table[i] = _map(
            _decay_release_table[i], 
            _decay_release_table[LUT_SIZE - 1], 
            _decay_release_table[0], 
            0, 
            _vertical_resolution - 1
        );
    }
}

void ADSR::set_reset_attack(bool l_reset_attack)
{
    _reset_attack = l_reset_attack;
}

void ADSR::set_attack(unsigned long l_attack)
{
    _attack = l_attack;
}

void ADSR::set_decay(unsigned long l_decay)
{
    _decay = l_decay;
}

void ADSR::set_sustain(int l_sustain)
{
    if (l_sustain < 0) {
        l_sustain = 0;
    }

    if (l_sustain >= _vertical_resolution) {
        l_sustain = _vertical_resolution - 1;
    }

    _sustain = l_sustain;
}

void ADSR::set_release(unsigned long l_release)
{
    _release = l_release;
}

void ADSR::note_on() {
    _t_note_on = _micros();                   // Set new timestamp for note_on

    // Set start value new Attack. If _reset_attack equals true, a new trigger starts with 0
    // otherwise start with the current output
    _attack_start = _reset_attack ? 0 : _adsr_output;
    
    _notes_pressed++;                               // increase number of pressed notes with one
}

void ADSR::note_off() {
    _notes_pressed--;
    if (_notes_pressed <= 0) {                      // if all notes are depressed - start release
        _t_note_off = _micros();                    // set timestamp for note off
        _release_start = _adsr_output;              // set start value for release
        _notes_pressed = 0;
    }
}

bool ADSR::is_on() {
    return _notes_pressed >= 1;
}

int ADSR::envelope()
{
    unsigned long delta = 0;
    
    // if note is pressed
    if (_t_note_off < _t_note_on) {
        delta = _micros() - _t_note_on;
        
         // Attack
        if (delta < _attack) {
            _adsr_output = _map(
                _attack_table[(int)floor(LUT_SIZE * (float) delta / (float)_attack)], 
                0, 
                _vertical_resolution - 1, 
                _attack_start, 
                _vertical_resolution - 1
            );

        // Decay
        } else if (delta < _attack + _decay) {         
            delta = _micros() - _t_note_on - _attack;
            _adsr_output = _map(
                _decay_release_table[(int)floor(LUT_SIZE * (float) delta / (float) _decay)], 
                0, 
                _vertical_resolution - 1, 
                _sustain, 
                _vertical_resolution - 1
            );
        
        // Sustain is reached
        } else {
            _adsr_output = _sustain;
        }
    }

    // if note not pressed
    if (_t_note_off > _t_note_on) {
        delta = _micros() - _t_note_off;
        
        // Release
        if (delta < _release) {
            _adsr_output = _map(
                _decay_release_table[(int)floor(LUT_SIZE * (float) delta / (float) _release)], 
                0, 
                _vertical_resolution - 1, 
                0, 
                _release_start);
        
        // Release finished
        } else {
            _adsr_output = 0;
        }
    }

    return _adsr_output;
}