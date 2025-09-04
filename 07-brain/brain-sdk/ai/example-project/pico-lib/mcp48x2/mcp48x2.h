/**
 * @file mcp48x2.h
 * @author Peter Zimon (peterzimon.com)
 * @brief  
 * 
 * Raspberry Pi Pico driver for Microchip MCP4802/MCP4812/MCP4822
 * Docs: https://github.com/peterzimon/pico-lib/tree/main/mcp48x2
 * 
 * @version 0.1
 * @date 2022-01-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef _PICO_LIB_MCP48X2_H
#define _PICO_LIB_MCP48X2_H

#include <pico/stdlib.h>
#include <hardware/spi.h>

enum mcp48x2_channel { MCP48X2_CHANNEL_A, MCP48X2_CHANNEL_B };
enum mcp48x2_gain { MCP48X2_GAIN_X2, MCP48X2_GAIN_X1 };

class MCP48X2 {
public:
    void init(spi_inst_t *spi, uint8_t pin_cs, uint8_t pin_sck, uint8_t pin_tx);
    void config(mcp48x2_channel channel, mcp48x2_gain gain, bool active);
    void set_channel(mcp48x2_channel channel);
    void set_gain(mcp48x2_gain gain);
    void set_active(bool active);
    void write(uint16_t value);

private: 
    uint8_t m_config;
    spi_inst_t *m_spi_port;
    uint8_t m_pin_cs;
    
    void m_cs_select();
    void m_cs_deselect();
};

#endif