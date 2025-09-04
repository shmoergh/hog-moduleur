#ifndef _SETTINGS_H
#define _SETTINGS_H

#include <iostream>
#include <string>
#include <cstring>
#include <inttypes.h>
#include <pico/stdio.h>
#include "hardware/spi.h"
#include "hardware/uart.h"
#include "hardware/flash.h"
#include "hardware/sync.h"

/**
 * Contains constants and enums.
 *
 * Note that all "GP_..." constant refers to the GP port of the Raspberry Pi
 * Pico, not the actual physical pin. Example:
 *
 *      GP_DAC_CS 5 means GP5 = physical pin 7
 *
 * Calibration
 * -----------
 * Since all electrical components have slightly different characteristics, the
 * final output voltage may differ from voice to voice (CV+Gate pairs). Follow the
 * process below to calibrate each voice's CV output:
 *
 * 0. According to the V/octave standard, all C's should output a rounded voltage
 *    value. C1 (ie. the 2nd octave C, MIDI note 12) should generate 1V, C2 should
 *    2V and so on. Midipoly can output 10 octaves so the first 10 octave on your
 *    MIDI keyboard should output 1, 2 ... 10 volts respectively.
 * 1. Use a precision multimeter and measure the CV output voltage. Note that
 *    when you don't press a key the output voltage is not precisely 0V due to
 *    imperfection and leakage current. This is something you can't calibrate.
 * 2. Press C1 on your MIDI keyboard. You should read around 1V on the multimeter.
 *    If it's higher than 1V then you'll need to set an adjustment value for the
 *    1st voice's 1st octave C. In order to do this, open cvgate.cpp and set the
 *    _cal[0][1] to some negative value. Setting it to -1 will case a about 2mV
 *    drop in the output. So _cal[0][1] sets the first voice ("0" -> first voice)
 *    first octave ("1" that's supposed to be 1V).
 * 3. Upload the firmware and check the output voltage again. If it's 1-2mV close
 *    then you can go to the next octave. Do this until all the octaves (0-10)
 *    show the appropriate voltage on the given voice.
 *
 * calibration[Cx] = RELATIVE ADJUSTMENT VALUE
 *              ^               ^
 *              ^               Use negative values to decrease the CV output
 *              C note on given octave. Should output xV (C1 -> 1V, C2 -> 2V etc.) [0-10]
 */

// DAC
#define DAC_SPI_PORT        spi0
#define GP_DAC_CS           5
#define GP_DAC_SCK          6
#define GP_DAC_MOSI         7

// MIDI
#define LOWEST_MIDI_NOTE    0x00
#define OCTAVES             10
#define MAX_PITCH_BEND      0x3fff
#define PITCH_BEND_CENTER   0x2000
#define MAX_PB_SEMINOTES    2

#define MIDI_OCTAVE_SHIFT   0

#define MIDI_UART_INSTANCE  uart1
#define GP_MIDI_RX          9
#define MIDI_BAUDRATE       31250

// CV
#define MAX_NOTE_VOLTAGE        4095
#define MAX_MODWHEEL_VOLTAGE    4095
#define GP_GATE                 21

// LED
#define GP_LED              10

/**
 * Handling saving settings to flash memory
 * ----------------------------------------
 * Offset should be larger than the filesize. To get filesize run:
 * arm-none-eabi-size build/hog-midi2cv.elf
 */

#define FLASH_TARGET_OFFSET (512 * 1024) // Safe offset beyond program size

class Settings {
    public:
        void init();
        void set_midi_channel(uint8_t new_midi_channel);
        void save_settings();
        void load_settings();

        int calibration[11];
        uint8_t midi_channel;
};

#endif