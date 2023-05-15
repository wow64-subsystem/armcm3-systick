#pragma once

#include <atomic>

class KCriticalSection {
private:
  std::atomic_bool state = false;
public:
  KCriticalSection() { }

  KCriticalSection(bool initial_state) {
    this->state.store(initial_state);
  }

  ~KCriticalSection() {
    this->state.store(false);
  }

  void enter() {
    while (true) {
      bool expected = false;

      if (this->state.compare_exchange_strong(expected, true)) {
        break;
      }
    }
  }

  bool try_enter() {
    bool expected = false;

    if (this->state.compare_exchange_strong(expected, true)) {
      return true;
    }

    return false;
  }
  
  void leave() {
    this->state.store(false);
  }

  bool get_state() {
    return this->state.load();
  }
};
