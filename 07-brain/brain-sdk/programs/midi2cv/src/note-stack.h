// NoteStack: order-preserving last-note priority buffer for MIDI notes

#ifndef MIDI2CV_NOTE_STACK_H
#define MIDI2CV_NOTE_STACK_H

#include <cstdint>
#include <optional>
#include <vector>

class NoteStack {
  public:
    void pushTop(uint8_t note);
    void remove(uint8_t note);
    std::optional<uint8_t> topOrNone() const;
    bool isEmpty() const { return notes_.empty(); }

  private:
    std::vector<uint8_t> notes_;
};

#endif  // MIDI2CV_NOTE_STACK_H

