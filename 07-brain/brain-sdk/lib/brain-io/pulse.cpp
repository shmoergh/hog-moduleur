// Implementation of Pulse class for hardware-inverted digital I/O.
// Handles GPIO setup, edge detection, and interrupt-driven callbacks.
// Safe initialization prevents output glitches during startup.

#include "pulse.h"

#include <algorithm>

#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "pico/time.h"
#include "pico/types.h"

namespace brain::io {

// Static instance tracking for IRQ handling
static Pulse* irq_instances[NUM_BANK0_GPIOS] = {nullptr};

Pulse::Pulse(uint in_gpio, uint out_gpio) :
	in_gpio_(in_gpio),
	out_gpio_(out_gpio),
	last_logical_state_(false),
	current_output_state_(false),
	glitch_filter_us_(0),
	interrupts_enabled_(false),
	last_change_time_us_(0),
	filtered_state_(false) {}

void Pulse::begin() {
	// Configure input pin with pull-up
	gpio_init(in_gpio_);
	gpio_set_dir(in_gpio_, GPIO_IN);
	gpio_pull_up(in_gpio_);

	// Configure output pin - set HIGH before switching to output to avoid glitch
	gpio_init(out_gpio_);
	gpio_put(out_gpio_, true);	// Set to idle (HIGH) first
	gpio_set_dir(out_gpio_, GPIO_OUT);

	// Initialize state
	last_logical_state_ = read();
	filtered_state_ = last_logical_state_;
	last_change_time_us_ = time_us_32();

	// Register this instance for IRQ handling
	if (in_gpio_ < NUM_BANK0_GPIOS) {
		irq_instances[in_gpio_] = this;
	}
}

void Pulse::end() {
	// Disable interrupts if enabled
	if (interrupts_enabled_) {
		disableInterrupts();
	}

	// Clear IRQ instance
	if (in_gpio_ < NUM_BANK0_GPIOS) {
		irq_instances[in_gpio_] = nullptr;
	}

	// Return pins to input/high-impedance
	gpio_set_dir(in_gpio_, GPIO_IN);
	gpio_set_dir(out_gpio_, GPIO_IN);
	gpio_disable_pulls(in_gpio_);
	gpio_disable_pulls(out_gpio_);
}

bool Pulse::read() const {
	// Input is inverted by hardware transistor
	return !gpio_get(in_gpio_);
}

bool Pulse::readRaw() const {
	return gpio_get(in_gpio_);
}

void Pulse::set(bool on) {
	// Only change if different from current state (idempotent)
	if (on != current_output_state_) {
		current_output_state_ = on;
		// Output stage is active-low: true = LOW, false = HIGH
		gpio_put(out_gpio_, !on);
	}
}

bool Pulse::get() const {
	return current_output_state_;
}

void Pulse::onRise(std::function<void()> cb) {
	on_rise_callback_ = cb;
}

void Pulse::onFall(std::function<void()> cb) {
	on_fall_callback_ = cb;
}

void Pulse::poll() {
	bool current_logical = read();

	// Apply glitch filtering if enabled
	if (glitch_filter_us_ > 0) {
		uint32_t now = time_us_32();

		if (current_logical != filtered_state_) {
			// State changed - start/continue filtering
			if (current_logical != last_logical_state_) {
				// New change - reset timer
				last_change_time_us_ = now;
			} else if ((now - last_change_time_us_) >= glitch_filter_us_) {
				// Change has been stable long enough
				filtered_state_ = current_logical;
			}
		}

		current_logical = filtered_state_;
	}

	// Detect edges and fire callbacks
	if (current_logical != last_logical_state_) {
		if (current_logical && on_rise_callback_) {
			on_rise_callback_();
		} else if (!current_logical && on_fall_callback_) {
			on_fall_callback_();
		}

		last_logical_state_ = current_logical;
	}
}

void Pulse::setInputGlitchFilterUs(uint32_t us) {
	glitch_filter_us_ = us;
}

void Pulse::enableInterrupts() {
	if (!interrupts_enabled_) {
		gpio_set_irq_enabled_with_callback(
			in_gpio_, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpioIrqHandler);
		interrupts_enabled_ = true;
	}
}

void Pulse::disableInterrupts() {
	if (interrupts_enabled_) {
		gpio_set_irq_enabled(in_gpio_, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, false);
		interrupts_enabled_ = false;
	}
}

void Pulse::gpioIrqHandler(uint gpio, uint32_t events) {
	if (gpio < NUM_BANK0_GPIOS && irq_instances[gpio] != nullptr) {
		// In ISR context - just record that an edge occurred
		// The actual edge processing happens in Poll() in main loop
		bool raw_state = gpio_get(gpio);
		irq_instances[gpio]->handleEdge(raw_state);
	}
}

void Pulse::handleEdge(bool raw_state) {
	// This is called from ISR - keep it minimal
	// The actual callback invocation happens in Poll()
	// Just update timing for glitch filter
	last_change_time_us_ = time_us_32();
}

}  // namespace brain::io
