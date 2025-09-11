#include "brain-io/audio-cv-out.h"

#include <hardware/gpio.h>
#include <pico/stdlib.h>

#include <cstdio>

namespace brain::io {

// MCP4822 command bits
static constexpr uint8_t kMCP4822_CHANNEL_A = 0;  // A/B = 0
static constexpr uint8_t kMCP4822_CHANNEL_B = 1;  // A/B = 1
static constexpr uint8_t kMCP4822_GAIN = 0;
static constexpr uint8_t kMCP4822_ACTIVE = 1;

// Voltage conversion constants
static constexpr float kMaxVoltage = 10.0f;
static constexpr uint16_t kMaxDacValue = 4095;
static constexpr uint32_t kSpiFrequency = 1000000;	// 1 MHz

bool AudioCvOut::init(spi_inst_t* spi_instance, uint cs_pin, uint sck_pin, uint tx_pin,
	uint coupling_pin_a, uint coupling_pin_b) {
	// Validate SPI instance
	if (spi_instance != spi0 && spi_instance != spi1) {
		fprintf(stderr, "AudioCvOut: Invalid SPI instance\n");
		return false;
	}

	// Store configuration
	spi_instance_ = spi_instance;
	cs_pin_ = cs_pin;
	sck_pin_ = sck_pin;
	tx_pin_ = tx_pin;
	coupling_pin_a_ = coupling_pin_a;
	coupling_pin_b_ = coupling_pin_b;

	// Initialize SPI
	spi_init(spi_instance_, kSpiFrequency);
	// spi_set_format(spi_instance_, 16, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);

	// Configure SPI pins (SCK and TX/MOSI) for SPI function
	gpio_set_function(sck_pin_, GPIO_FUNC_SPI);
	gpio_set_function(tx_pin_, GPIO_FUNC_SPI);

	// Configure CS pin
	gpio_init(cs_pin_);
	gpio_set_dir(cs_pin_, GPIO_OUT);
	gpio_put(cs_pin_, 1);  // CS idle high

	// Configure coupling control pins
	gpio_init(coupling_pin_a_);
	gpio_set_dir(coupling_pin_a_, GPIO_OUT);
	gpio_put(coupling_pin_a_, 0);  // Default to DC coupling

	gpio_init(coupling_pin_b_);
	gpio_set_dir(coupling_pin_b_, GPIO_OUT);
	gpio_put(coupling_pin_b_, 0);  // Default to DC coupling

	return true;
}

bool AudioCvOut::setVoltage(AudioCvOutChannel channel, float voltage) {
	// Validate voltage range
	if (voltage < 0.0f || voltage > kMaxVoltage) {
		fprintf(stderr, "AudioCvOut: Voltage %.2fV out of range (0-%.1fV)\n", voltage, kMaxVoltage);
		return false;
	}

	// Convert voltage to DAC value and send command
	uint16_t dac_value = voltageToDAC(voltage);
	writeDacChannel(channel, dac_value);
	return true;
}

bool AudioCvOut::setCoupling(AudioCvOutChannel channel, AudioCvOutCoupling coupling) {
	uint coupling_pin =
		(channel == AudioCvOutChannel::kChannelA) ? coupling_pin_a_ : coupling_pin_b_;

	// Set coupling: 0 = DC, 1 = AC
	gpio_put(coupling_pin, static_cast<bool>(coupling));
	return true;
}

void AudioCvOut::writeDacChannel(AudioCvOutChannel channel, uint16_t dac_value) {
	// Constructing DAC config
	uint8_t config =
		(channel == AudioCvOutChannel::kChannelA ? kMCP4822_CHANNEL_A : kMCP4822_CHANNEL_B) << 3 |
		0 << 2 | kMCP4822_GAIN << 1 | kMCP4822_ACTIVE;

	// Get hi-byte
	uint8_t data[2];
	data[0] = config << 4 | (dac_value & 0xf00) >> 8;

	// Get lo-byte
	data[1] = dac_value & 0xff;

	// Send command via SPI
	asm volatile("nop \n nop \n nop");
	gpio_put(cs_pin_, 0);  // Assert CS
	asm volatile("nop \n nop \n nop");

	spi_write_blocking(spi_instance_, data, 2);

	asm volatile("nop \n nop \n nop");
	gpio_put(cs_pin_, 1);  // Deassert CS
	asm volatile("nop \n nop \n nop");
}

uint16_t AudioCvOut::voltageToDAC(float voltage) {
	// Linear conversion: 0V -> 0, 10V -> 4095
	float normalized = voltage / kMaxVoltage;
	uint16_t dac_value = static_cast<uint16_t>(normalized * kMaxDacValue + 0.5f);

	// Ensure we don't exceed 12-bit range
	return (dac_value > kMaxDacValue) ? kMaxDacValue : dac_value;
}

}  // namespace brain::io
