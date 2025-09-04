#include "settings.h"

void Settings::init() {
    calibration[0] = 0;
    calibration[1] = 0;
    calibration[2] = 0;
    calibration[3] = 0;
    calibration[4] = 0;
    calibration[5] = 0;
    calibration[6] = 0;
    calibration[7] = 0;
    calibration[8] = 0;
    calibration[9] = 0;
    calibration[10] = 0;

    load_settings();

    // Saving default midi channel to flash memory if it's not set
    if (midi_channel < 1 || midi_channel > 16) {
        midi_channel = 1;
        save_settings();
    }

    printf("MIDI channel: %d\r\n", midi_channel);
}

void Settings::set_midi_channel(uint8_t new_midi_channel) {
    if (new_midi_channel >= 1) {
        if (new_midi_channel > 16) {
            new_midi_channel = 16;
        }
        midi_channel = new_midi_channel;
    }
    save_settings();
    printf("New MIDI channel: %d\r\n", midi_channel);
}

void Settings::save_settings() {
    // Ensure the data to write is aligned to the flash page size
    uint8_t data[FLASH_PAGE_SIZE];
    memset(data, 0xFF, FLASH_PAGE_SIZE); // Fill with 0xFF (erased state)
    data[0] = midi_channel;

    uint32_t ints = save_and_disable_interrupts();
    flash_range_erase(FLASH_TARGET_OFFSET, FLASH_SECTOR_SIZE);
    flash_range_program(FLASH_TARGET_OFFSET, data, FLASH_PAGE_SIZE);
    restore_interrupts(ints);
}

void Settings::load_settings() {
    const uint8_t *flash_data = (const uint8_t *)(XIP_BASE + FLASH_TARGET_OFFSET);
    midi_channel = flash_data[0];
}