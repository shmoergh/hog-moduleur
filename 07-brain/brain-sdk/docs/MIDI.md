# MIDI Parser (`brain::io::MidiParser`)
Transport-agnostic MIDI parser for channel voice messages. Supports:
- **Note On/Off** messages with velocity
- **Control Change** (CC) messages
- **Pitch Bend** messages (signed 14-bit range: -8192 to +8191)
- **Real-time** messages (timing clock, etc.)
- **Channel filtering** (1-16) with optional Omni mode
- **Running status** support per MIDI specification
- **ISR-safe** `feed()` method for real-time parsing

## Basic Usage
```cpp
#include <brain-io/midi_parser.h>

void onNoteOn(uint8_t note, uint8_t velocity, uint8_t channel) {
    printf("Note On: %d, vel: %d, ch: %d\n", note, velocity, channel);
}

brain::io::MidiParser parser(1, false);  // Channel 1, not omni
parser.setNoteOnCallback(onNoteOn);

// Feed MIDI bytes (e.g., from UART ISR)
parser.feed(0x90);  // Note On, channel 1
parser.feed(60);    // Middle C
parser.feed(64);    // Velocity 64
```

The parser handles interleaved real-time messages and maintains proper MIDI state machine behavior. It's designed to work with any MIDI transport (UART DIN, USB-MIDI) after byte extraction.