#include "mcp48x2.h"

/**
 * Init driver
 */
void MCP48X2::init(spi_inst_t *spi_port, uint8_t pin_cs, uint8_t pin_sck, uint8_t pin_tx) {
    m_spi_port = spi_port;
    m_pin_cs = pin_cs;
    
    // Set default configuration: channel A, 2x gain, activate chip
    config(MCP48X2_CHANNEL_A, MCP48X2_GAIN_X2, 1);

    // Let's use 1MHz
    spi_init(m_spi_port, 1000 * 1000);
    gpio_set_function(pin_sck, GPIO_FUNC_SPI);
    gpio_set_function(pin_tx, GPIO_FUNC_SPI);

    // Set chip select high by default
    gpio_init(m_pin_cs);
    gpio_set_dir(m_pin_cs, GPIO_OUT);
    gpio_put(m_pin_cs, 1);
}

/**
 * Configure and re-configure the DAC anytime during running
 * 
 * @param channel Selects channel (dacA | dacB)
 * @param gain Sets gain (1x | 2x)
 * @param active Sets chip active (1 | 0)
 */
void MCP48X2::config(mcp48x2_channel channel, mcp48x2_gain gain, bool active) {
    m_config = channel << 3 | 0 << 2 | gain << 1 | active;
}

/**
 * @brief Sets channel to write to
 * 
 * @param channel (dacA | dacB)
 */
void MCP48X2::set_channel(mcp48x2_channel channel) {
    m_config = channel << 3 & m_config;
}

/**
 * @brief Sets gain 
 * 
 * @param gain (1x | 2x)
 */
void MCP48X2::set_gain(mcp48x2_gain gain) {
    m_config = gain << 1 & m_config;
}

/**
 * @brief Sets active (~shutdown) flag
 * 
 * @param active (true | false)
 */
void MCP48X2::set_active(bool active) {
    m_config = active & m_config;
}

/**
 * Writes a value to the DAC based on the given config
 * 
 * @param value Da 12bit value!
 */
void MCP48X2::write(uint16_t value) {
    uint8_t data[2];

    // Get hi-byte
    data[0] = m_config << 4 | (value & 0xf00) >> 8;

    // Get lo-byte
    data[1] = value & 0xff;

    // Transmit the data
    m_cs_select();
    spi_write_blocking(m_spi_port, data, 2);
    m_cs_deselect();
}

/**
 * Select chip
 */
void MCP48X2::m_cs_select() {
    asm volatile("nop \n nop \n nop");
    gpio_put(m_pin_cs, 0);  // Active low
    asm volatile("nop \n nop \n nop");
}

/**
 * Deselect chip
 */
void MCP48X2::m_cs_deselect() {
    asm volatile("nop \n nop \n nop");
    gpio_put(m_pin_cs, 1);
    asm volatile("nop \n nop \n nop");
}