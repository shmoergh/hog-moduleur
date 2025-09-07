// Button input handler with debounce, long press, and single-tap detection.
// Provides callback system for user input events on Brain module hardware.
// Requires: GPIO pin with pull-up/pull-down configuration.

#ifndef BRAIN_UI_BUTTON_H_
#define BRAIN_UI_BUTTON_H_

#include <cstdint>
#include <functional>

#include "pico/stdlib.h"

namespace brain::ui {

/**
 * @brief Button input handler with debounce, long press, and single-tap detection.
 *
 * Provides a robust button interface with software debouncing and multiple
 * event types. Handles typical button bounce issues and provides callbacks
 * for different interaction patterns.
 */
class Button {
	public:
	/**
	 * @brief Construct a new Button object
	 *
	 * @param gpio_pin GPIO pin number for button input
	 * @param debounce_ms Debounce time in milliseconds (default: 50ms)
	 * @param long_press_ms Long press threshold in milliseconds (default: 500ms)
	 */
	Button(uint gpio_pin, uint32_t debounce_ms = 50, uint32_t long_press_ms = 500);

	/**
	 * @brief Initialize GPIO pin with pull-up or pull-down resistor
	 *
	 * @param pull_up true for pull-up (button connects to GND), false for pull-down
	 */
	void init(bool pull_up = true);

	/**
	 * @brief Poll button state and trigger callbacks (call in main loop)
	 *
	 * This method handles debouncing, edge detection, and timing for
	 * long press detection. Must be called regularly for proper operation.
	 */
	void update();

	/**
	 * @brief Set callback for button press event
	 *
	 * @param callback Function to invoke when button is pressed (after debounce)
	 */
	void setOnPress(std::function<void()> callback);

	/**
	 * @brief Set callback for button release event
	 *
	 * @param callback Function to invoke when button is released (after debounce)
	 */
	void setOnRelease(std::function<void()> callback);

	/**
	 * @brief Set callback for single tap event
	 *
	 * @param callback Function to invoke for quick press-release cycles
	 */
	void setOnSingleTap(std::function<void()> callback);

	/**
	 * @brief Set callback for long press event
	 *
	 * @param callback Function to invoke when button is held beyond threshold
	 */
	void setOnLongPress(std::function<void()> callback);

	private:
	uint gpio_pin_;	 ///< GPIO pin number for button input
	bool is_pressed_;  ///< Current debounced button state
	absolute_time_t last_press_time_;  ///< Timestamp of last press event
	absolute_time_t last_release_time_;	 ///< Timestamp of last release event
	absolute_time_t last_tap_time_;	 ///< Timestamp of last tap for double-tap detection
	uint32_t debounce_ms_;	///< Debounce time in milliseconds
	uint32_t long_press_ms_;  ///< Long press threshold in milliseconds

	std::function<void()> on_press_;  ///< Callback for press events
	std::function<void()> on_release_;	///< Callback for release events
	std::function<void()> on_single_tap_;  ///< Callback for single tap events
	std::function<void()> on_long_press_;  ///< Callback for long press events

	bool long_press_triggered_;	 ///< Flag to prevent multiple long press events
};

}  // namespace brain::ui

#endif	// BRAIN_UI_BUTTON_H_
