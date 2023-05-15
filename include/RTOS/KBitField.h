#pragma once

namespace RTOS {
  template<typename T>
  inline T toggle_bit(T value, T bit_index) {
    if (value & bit_index) {
      return value;
    }

    return value ^ bit_index;
  }

  template<typename T>
  inline T disable_bit(T value, T bit_index) {
    if (value & bit_index) {
      return value ^ bit_index;
    }

    return value;
  }
}
