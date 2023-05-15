#include <cstdint>
#include <atomic>
#include "HAL/HalSysTick.h"
#include "RTOS/KCriticalSection.h"
#include "RTOS/KBitField.h"

namespace HAL {
  namespace SysTick {
    static volatile uint32_t * ST_CTRL { reinterpret_cast<volatile uint32_t*>(0xE000E010) };
    static volatile uint32_t * ST_LOAD { reinterpret_cast<volatile uint32_t*>(0xE000E014) };
    static volatile uint32_t * ST_CURRENT { reinterpret_cast<volatile uint32_t*>(0xE000E018) };

    static constexpr volatile uint32_t ST_CTRL_CLK_SRC_SYSTEM_CLOCK { 1 << 2 };
    static constexpr volatile uint32_t ST_CTRL_CLK_SRC_EXTERNAL_CLOCK { 0 << 2 };
    static constexpr volatile uint32_t ST_CTRL_INTEN_TRUE { 1 << 1 };
    static constexpr volatile uint32_t ST_CTRL_INTEN_FALSE { 0 << 1 };
    static constexpr volatile uint32_t ST_CTRL_ENABLE { 1 << 0 };
    static constexpr volatile uint32_t ST_CTRL_DISABLE { 0 << 0 };

    static constexpr volatile uint32_t ST_CTRL_CLK_SRC_BIT { 0b100 };
    static constexpr volatile uint32_t ST_CTRL_INTEN_BIT { 0b010 };
    static constexpr volatile uint32_t ST_CTRL_ENABLE_BIT { 0b001 };

    static KCriticalSection lock { KCriticalSection(false) };

    /* prevents from configuring the clock when it's done by other task */
    void enter_configuration_lock() {
      HAL::SysTick::lock.enter();
    }

    /* prevents from configuring the clock when it's done by other task */
    void leave_configuration_lock() {
      HAL::SysTick::lock.leave();
    }

    static uint32_t get_control_register_value() {
      return reinterpret_cast<uint32_t>(*HAL::SysTick::ST_CTRL);
    }

    /* returns the previous value */
    static uint32_t store_control_register_value(uint32_t value) {
      uint32_t result { reinterpret_cast<uint32_t>(*HAL::SysTick::ST_CTRL) };

      *HAL::SysTick::ST_CTRL = value;

      return result;
    }

    /* sets the SysTick source */
    void set_source(SysTickSource source) {
      uint32_t value { HAL::SysTick::get_control_register_value() };

      switch (source) {
      case HAL::SysTick::SYSTICK_SOURCE_SYSTEM_CLOCK:
        value = RTOS::toggle_bit<uint32_t>(value, ST_CTRL_CLK_SRC_BIT);
        break;
      case HAL::SysTick::SYSTICK_SOURCE_EXTERNAL_CLOCK:
        value = RTOS::disable_bit<uint32_t>(value, ST_CTRL_CLK_SRC_BIT);
        break;
      default:
        // shouldn't reach here, it will throw an error on compiler time
        break;
      }
      
      HAL::SysTick::store_control_register_value(value);
    }

    /* enable raising of interrupts */
    void enable_interrupts() {
      uint32_t value { HAL::SysTick::get_control_register_value() };

      value = RTOS::toggle_bit<uint32_t>(value, ST_CTRL_INTEN_BIT);
      
      HAL::SysTick::store_control_register_value(value);
    }

    /* disables the raising of interrupts */
    void disable_interrupts() {
      uint32_t value { HAL::SysTick::get_control_register_value() };

      value = RTOS::disable_bit<uint32_t>(value, ST_CTRL_INTEN_BIT);
      
      HAL::SysTick::store_control_register_value(value);
    }

    /* enables SysTick */
    void enable() {
      uint32_t value { HAL::SysTick::get_control_register_value() };

      value = RTOS::toggle_bit<uint32_t>(value, ST_CTRL_ENABLE_BIT);
      
      HAL::SysTick::store_control_register_value(value);
    }

    /* disables SysTick */
    void disable() {
      uint32_t value { HAL::SysTick::get_control_register_value() };

      value = RTOS::disable_bit<uint32_t>(value, ST_CTRL_ENABLE_BIT);
      
      HAL::SysTick::store_control_register_value(value);
    }

    /* returns the previous value */
    uint32_t store_timer_value(uint32_t value) {
      uint32_t result { *HAL::SysTick::ST_LOAD };

      *HAL::SysTick::ST_LOAD = value;

      return result;
    }
  }
}
