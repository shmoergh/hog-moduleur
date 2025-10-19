// NoteStack implementation

#include "note-stack.h"

#include <algorithm>

void NoteStack::pushTop(uint8_t note) {
    // Move to top if exists, else append
    auto it = std::find(notes_.begin(), notes_.end(), note);
    if (it != notes_.end()) {
        // Move existing to top: erase then push_back
        notes_.erase(it);
    }
    notes_.push_back(note);
}

void NoteStack::remove(uint8_t note) {
    auto it = std::find(notes_.begin(), notes_.end(), note);
    if (it != notes_.end()) {
        notes_.erase(it);
    }
}

std::optional<uint8_t> NoteStack::topOrNone() const {
    if (notes_.empty()) return std::nullopt;
    return notes_.back();
}

