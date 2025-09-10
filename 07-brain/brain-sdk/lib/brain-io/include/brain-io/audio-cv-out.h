// Audio/CV output via MCP4822 DAC with DC/AC coupling control
// Dependencies: SPI, GPIO. Hardware: MCP4822 dual DAC, CD4053 analog switch
// Controls voltage output 0-10V on channels A/B with switchable DC/AC coupling
// Pin ownership: SPI CS, two GPIO for coupling control
// Author: Brain SDK
#pragma once

#include <hardware/spi.h>

#include <cstdint>

#include "brain-common/brain-gpio-setup.h"

namespace brain::io {

/** DAC output channel selection */
enum class AudioCvOutChannel { kChannelA = 0, kChannelB = 1 };

/** Coupling mode for output stage */
enum class AudioCvOutCoupling {
	kDcCoupled = 0,	 // Direct coupling - full DC range
	kAcCoupled = 1	// AC coupling - blocks DC component
};

/** Audio/CV output controller for MCP4822 DAC with coupling switches */
class AudioCvOut {
	public:
	/**
	 * Initialize SPI interface and GPIO pins for DAC and coupling control
	 * @param spi_instance SPI peripheral instance (default: spi0)
	 * @param cs_pin Chip select GPIO pin for MCP4822 (default: GPIO 5)
	 * @param coupling_pin_a CD4053 control pin for channel A coupling (default: GPIO 6)
	 * @param coupling_pin_b CD4053 control pin for channel B coupling (default: GPIO 7)
	 * @return true if initialization successful, false on error
	 */
	bool init(spi_inst_t* spi_instance = spi0, uint cs_pin = GPIO_BRAIN_AUDIO_CV_OUT_CS,
		uint coupling_pin_a = GPIO_BRAIN_AUDIO_CV_OUT_COUPLING_A,
		uint coupling_pin_b = GPIO_BRAIN_AUDIO_CV_OUT_COUPLING_B);

	/**
	 * Set output voltage on specified channel
	 * @param channel Target output channel (A or B)
	 * @param voltage Output voltage in range 0.0V to 10.0V
	 * @return true if voltage set successfully, false on error
	 */
	bool setVoltage(AudioCvOutChannel channel, float voltage);

	/**
	 * Configure DC/AC coupling for specified channel
	 * @param channel Target output channel (A or B)
	 * @param coupling Desired coupling mode
	 * @return true if coupling set successfully, false on error
	 */
	bool setCoupling(AudioCvOutChannel channel, AudioCvOutCoupling coupling);

	private:
	/** Send 16-bit command to MCP4822 via SPI */
	void writeDacChannel(AudioCvOutChannel channel, uint16_t dac_value);

	/** Convert voltage (0-10V) to 12-bit DAC value (0-4095) */
	uint16_t voltageToDAC(float voltage);

	// Hardware configuration
	uint cs_pin_ = 0;
	uint coupling_pin_a_ = 0;
	uint coupling_pin_b_ = 0;
	spi_inst_t* spi_instance_ = nullptr;
};

}  // namespace brain::io