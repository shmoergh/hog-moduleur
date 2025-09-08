# MIDI Parser (`brain::io::MidiParser`)
MIDI parser with integrated UART input for channel voice messages. Supports:
- **Note On/Off** messages with velocity
- **Control Change** (CC) messages
- **Pitch Bend** messages (signed 14-bit range: -8192 to +8191)
- **Real-time** messages (timing clock, etc.)
- **Channel filtering** (1-16) with optional Omni mode
- **Running status** support per MIDI specification
- **ISR-safe** `feed()` method for real-time parsing
- **Integrated UART** support for easy MIDI DIN input

## Basic Usage

### With Integrated UART (Recommended)
```cpp
#include <brain-io/midi_parser.h>
#include <hardware/uart.h>

void onNoteOn(uint8_t note, uint8_t velocity, uint8_t channel) {
    printf("Note On: %d, vel: %d, ch: %d\n", note, velocity, channel);
}

brain::io::MidiParser parser(1, false);  // Channel 1, not omni
parser.setNoteOnCallback(onNoteOn);

// Initialize UART for MIDI input (31250 baud, GPIO 5 for RX)
if (parser.initUart(uart1, 5)) {
    // In main loop
    while (true) {
        parser.processUartInput();  // Process any incoming MIDI
        // ... other tasks
    }
}
```

### Transport-Agnostic (Advanced)
```cpp
brain::io::MidiParser parser(1, false);  // Channel 1, not omni
parser.setNoteOnCallback(onNoteOn);

// Feed MIDI bytes from any source (e.g., UART ISR, USB-MIDI)
parser.feed(0x90);  // Note On, channel 1
parser.feed(60);    // Middle C
parser.feed(64);    // Velocity 64
```

The parser handles interleaved real-time messages and maintains proper MIDI state machine behavior. The integrated UART approach simplifies MIDI DIN input setup, while the transport-agnostic `feed()` method allows integration with USB-MIDI or other transports.