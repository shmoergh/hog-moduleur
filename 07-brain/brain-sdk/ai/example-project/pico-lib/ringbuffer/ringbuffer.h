/*
 * Class: Super-simple™ Ring Buffer
 * --------------------------------
 * Stores one byte long data in array (pointer). Specifically made for UART
 * (MIDI) data buffering. 
 * ------------------------------------------------------------------------- */

#ifndef _PICO_LIB_RINGBUFFER_H
#define _PICO_LIB_RINGBUFFER_H

#include <pico/stdlib.h>

class RingBuffer {
public: 
    void init(uint8_t* dataBuffer, uint16_t bufferSize);

    bool write_byte(uint8_t data);
    bool read_byte(uint8_t& data);
    bool peek(uint8_t& data);
    bool is_full();
    bool is_empty();

private:
    uint8_t m_buffer_size;
    uint8_t* m_data_buffer;
    int m_read_index;
    int m_write_index;
    int m_count;
};

#endif