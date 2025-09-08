// Pulse input/output handler with hardware inversion support for Brain module.
// Provides digital I/O with transistor-inverted signals and edge detection.
// Requires: GPIO pins for input (pull-up) and output (active-low).

#ifndef BRAIN_IO_PULSE_H_
#define BRAIN_IO_PULSE_H_

#include <cstdint>
#include <functional>

#include "brain-gpio-setup.h"
#include "pico/types.h"

namespace brain::io {

/**
 * @brief Pulse input/output handler with hardware inversion support
 *
 * Provides a simple API for reading a transistor-inverted digital input
 * and driving a transistor-inverted digital output. The SDK handles
 * inversion transparently.
 */
class Pulse {
	public:
	/**
	 * @brief Construct a new Pulse object
	 *
	 * @param in_gpio GPIO pin number for input (default: GPIO_BRAIN_PULSE_INPUT)
	 * @param out_gpio GPIO pin number for output (default: GPIO_BRAIN_PULSE_OUTPUT)
	 */
	Pulse(uint in_gpio = GPIO_BRAIN_PULSE_INPUT, uint out_gpio = GPIO_BRAIN_PULSE_OUTPUT);

	/**
	 * @brief Initialize GPIO pins and set safe output state
	 */
	void begin();

	/**
	 * @brief Return pins to input/high-impedance state
	 */
	void end();

	/**
	 * @brief Read logical input state (hardware inversion handled)
	 *
	 * @return true when physical jack is logically ON
	 */
	bool read() const;

	/**
	 * @brief Read raw GPIO level for debugging
	 *
	 * @return true when GPIO pin is HIGH
	 */
	bool readRaw() const;

	/**
	 * @brief Set logical output state
	 *
	 * @param on true to assert output (active), false to de-assert (idle)
	 */
	void set(bool on);

	/**
	 * @brief Get last commanded logical output state
	 *
	 * @return true if output was set to active, false if idle
	 */
	bool get() const;

	/**
	 * @brief Set callback for logical rising edge (low→high)
	 *
	 * @param cb Callback function to invoke
	 */
	void onRise(std::function<void()> cb);

	/**
	 * @brief Set callback for logical falling edge (high→low)
	 *
	 * @param cb Callback function to invoke
	 */
	void onFall(std::function<void()> cb);

	/**
	 * @brief Poll for edge detection (call in main loop)
	 */
	void poll();

	/**
	 * @brief Set input glitch filter duration
	 *
	 * @param us Microseconds to filter (0 = disabled)
	 */
	void setInputGlitchFilterUs(uint32_t us);

	/**
	 * @brief Enable interrupt-driven edge detection
	 */
	void enableInterrupts();

	/**
	 * @brief Disable interrupt-driven edge detection
	 */
	void disableInterrupts();

	private:
	uint in_gpio_;
	uint out_gpio_;
	bool last_logical_state_;
	bool current_output_state_;
	uint32_t glitch_filter_us_;
	bool interrupts_enabled_;

	std::function<void()> on_rise_callback_;
	std::function<void()> on_fall_callback_;

	// For glitch filtering
	uint32_t last_change_time_us_;
	bool filtered_state_;

	static void gpioIrqHandler(uint gpio, uint32_t events);
	void handleEdge(bool raw_state);
};

}  // namespace brain::io

#endif	// BRAIN_IO_PULSE_H_
