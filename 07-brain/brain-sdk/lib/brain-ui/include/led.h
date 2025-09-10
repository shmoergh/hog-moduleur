#pragma once

#include <pico/stdlib.h>

#include <cstdint>
#include <functional>

namespace brain::ui {

/**
 * @brief LED handler for PWM brightness, blinking, and state callbacks.
 *
 * Provides PWM-based brightness control and various blinking patterns with
 * callback support. Handles transistor-driven LEDs with configurable brightness
 * and timing patterns for user feedback.
 */
class Led {
	public:
	/**
	 * @brief Construct a new LED object
	 *
	 * @param gpio_pin GPIO pin number connected to LED driver transistor
	 */
	Led(uint gpio_pin);

	/**
	 * @brief Initialize GPIO pin and PWM slice for brightness control
	 */
	void init();

	/**
	 * @brief Turn LED on at current brightness level
	 */
	void on();

	/**
	 * @brief Turn LED off (0% brightness)
	 */
	void off();

	/**
	 * @brief Set LED brightness level
	 *
	 * @param value Brightness level (0-255, where 255 is maximum brightness)
	 */
	void setBrightness(uint8_t value);

	/**
	 * @brief Blink LED a specific number of times
	 *
	 * @param times Number of blinks to perform
	 * @param interval_ms Time between state changes in milliseconds
	 */
	void blink(uint times, uint interval_ms);

	/**
	 * @brief Blink LED for a specific duration
	 *
	 * @param duration_ms Total blinking duration in milliseconds
	 * @param interval_ms Time between state changes in milliseconds
	 */
	void blinkDuration(uint duration_ms, uint interval_ms);

	/**
	 * @brief Start continuous blinking (until stopped manually)
	 *
	 * @param interval_ms Time between state changes in milliseconds
	 */
	void startBlink(uint interval_ms);

	/**
	 * @brief Stop any active blinking pattern
	 */
	void stopBlink();

	/**
	 * @brief Update LED state and handle timing (call in main loop)
	 *
	 * This method manages blink timing and triggers callbacks.
	 * Must be called regularly for proper blink operation.
	 */
	void update();

	/**
	 * @brief Set callback for LED state changes
	 *
	 * @param callback Function to invoke when LED state changes (on/off)
	 */
	void setOnStateChange(std::function<void(bool)> callback);

	/**
	 * @brief Set callback for end of blink sequence
	 *
	 * @param callback Function to invoke when blink pattern completes
	 */
	void setOnBlinkEnd(std::function<void()> callback);

	/**
	 * @brief Check if LED is currently on
	 *
	 * @return true if LED is on, false if off
	 */
	bool isOn() const;

	/**
	 * @brief Check if LED is currently blinking
	 *
	 * @return true if any blink pattern is active, false otherwise
	 */
	bool isBlinking() const;

	private:
	uint gpio_pin_;	 ///< GPIO pin number for LED output
	uint8_t brightness_;  ///< Current brightness level (0-255)
	bool state_;  ///< Current LED state (on/off)
	bool blinking_;	 ///< True if any blink pattern is active
	bool constant_blink_;  ///< True for continuous blinking mode
	uint blink_times_;	///< Number of blinks remaining for counted blink
	uint blink_interval_ms_;  ///< Time between blink state changes
	uint blink_count_;	///< Current blink count for tracking
	absolute_time_t last_blink_time_;  ///< Timestamp of last blink state change
	std::function<void(bool)> on_state_change_;	 ///< Callback for state change events
	std::function<void()> on_blink_end_;  ///< Callback for blink sequence completion
	// For blinkDuration
	bool duration_blink_ = false;  ///< True for duration-based blinking
	uint duration_ms_ = 0;	///< Total duration for duration-based blink
	absolute_time_t blink_start_time_ = 0;	///< Start time for duration-based blink
};

}  // namespace brain::ui
