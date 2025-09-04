#include "ringbuffer.h"

/*
 * Initializes the buffer variables.
 * 
 * databuffer: a pointer to the databuffer array
 * bufferSize: size of the buffer
 * no return
 * ------------------------------------------------------------------------- */
void RingBuffer::init(uint8_t* data_buffer, uint16_t buffer_size) {
    this->m_buffer_size = buffer_size;
    this->m_write_index = 0;
    this->m_read_index = 0;
    this->m_count = 0;
    this->m_data_buffer = data_buffer;
}

/*
 * Writes a single byte to the buffer. There's no length input, keep it simple.
 * 
 * data: a byte long data to be written to the buffer. If the buffer is full
 *       (ie. the data has not been read from it) then write is not executed.
 * returns: true if write is successful (buffer isn't full), false if the 
 *          buffer is full and write didn't happen 
 * ------------------------------------------------------------------------- */
bool RingBuffer::write_byte(uint8_t data) {
    if (!is_full()) {
        this->m_data_buffer[m_write_index] = data;          // Write data to current index
        m_write_index = (m_write_index + 1) % m_buffer_size;  // Increase write index
        m_count++;                                       // Increase number of data
        return true;
    }
    return false;
}

/*
 * Reads a single byte from the buffer. When done, step the read index.
 * 
 * data: pointer to the data byte variable where the data should be read to.
 * returns: true if the buffer is not empty (ie. not all data has been read),
 *          false if the all the data from the buffer has been read already
 * ------------------------------------------------------------------------- */
bool RingBuffer::read_byte(uint8_t& data) {
    if (!is_empty()) {
        data = this->m_data_buffer[m_read_index];
        m_read_index = (m_read_index + 1) % m_buffer_size;
        m_count--;
        return true;
    }
    return false;
}

/*
 * Checks what's the very next byte to be read without advancing the read index.
 * 
 * data: the pointer where the data should be read to
 * returns: true if the buffer is not empty (ie. not all data has been read),
 *          false if the all the data from the buffer has been read already
 * ------------------------------------------------------------------------- */
bool RingBuffer::peek(uint8_t& data) {
    if (!is_empty()) {
        data = this->m_data_buffer[m_read_index];
        return true;
    }
    return false;
}

/*
 * Checks if the buffer is full.
 * 
 * returns: true if the buffer is full, false if not
 * ------------------------------------------------------------------------- */
bool RingBuffer::is_full() {
    return m_count == m_buffer_size;
}

/*
 * Checks if the buffer is empty.
 * 
 * returns: true if the buffer is empty, false if not
 * ------------------------------------------------------------------------- */
bool RingBuffer::is_empty() {
    return m_count == 0;
}