#!/bin/bash

# new-program.sh - Create a new Pico program with boilerplate

set -e

if [ $# -ne 1 ]; then
  echo "Usage: ./scripts/new-program.sh <program-name>"
  exit 1
fi

PROG_NAME="$1"
PROG_DIR="$(dirname "$0")/../programs/$PROG_NAME"

if [ -d "$PROG_DIR" ]; then
  echo "Error: Program folder '$PROG_DIR' already exists."
  exit 1
fi

mkdir -p "$PROG_DIR"

cat > "$PROG_DIR/CMakeLists.txt" <<EOF
cmake_minimum_required(VERSION 3.22)
project($PROG_NAME)
add_executable($PROG_NAME main.cpp)
target_link_libraries($PROG_NAME
	pico_stdlib
	brain-common
	brain-ui
	brain-io)
pico_enable_stdio_usb($PROG_NAME 1)
pico_enable_stdio_uart($PROG_NAME 1)
pico_add_extra_outputs($PROG_NAME)
EOF

cat > "$PROG_DIR/main.cpp" <<EOF
#include <pico/stdlib.h>

const uint LED_PIN = PICO_DEFAULT_LED_PIN;

int main() {
	stdio_init_all();
	gpio_init(LED_PIN);
	gpio_set_dir(LED_PIN, GPIO_OUT);
	while (true) {
		gpio_put(LED_PIN, 1);
		sleep_ms(500);
		gpio_put(LED_PIN, 0);
		sleep_ms(500);
	}
	return 0;
}
EOF

echo "Created program directory: $PROG_DIR"
echo "Created: $PROG_DIR/CMakeLists.txt"
echo "Created: $PROG_DIR/main.cpp"
echo "Success! New program created at: $PROG_DIR"
echo ""
echo "Reminder: Re-run CMake configure/build so the new program is detected."
echo "For example:"
echo "  rm -rf build && cmake -B build -G 'Unix Makefiles' && cmake --build build"
