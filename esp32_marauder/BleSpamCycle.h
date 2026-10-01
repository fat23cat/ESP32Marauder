#pragma once

#include <stdint.h>

// Keep each UI iteration short enough to sample the Cardputer keyboard between
// advertisements. The caller maps the selected index to its payload type.
class BleSpamCycle {
 public:
  static constexpr uint8_t kPayloadCount = 6;

  template <class Run>
  void runNext(Run run) {
    run(next_);
    next_ = static_cast<uint8_t>((next_ + 1) % kPayloadCount);
  }

  void reset() { next_ = 0; }

 private:
  uint8_t next_ = 0;
};
