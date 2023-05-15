#pragma once

namespace HAL {
  namespace SysTick {
    enum SysTickSource {
      SYSTICK_SOURCE_SYSTEM_CLOCK,
      SYSTICK_SOURCE_EXTERNAL_CLOCK,
    };

    /* prevents from configuring the clock when it's done by other task */
    void enter_configuration_lock();

    /* prevents from configuring the clock when it's done by other task */
    void leave_configuration_lock();

    /* sets the SysTick source */
    void set_source(SysTickSource source);
    
    /* enable raising of interrupts */
    void enable_interrupts();
    
    /* disables the raising of interrupts */
    void disable_interrupts();
    
    /* enables SysTick */
    void enable();

    /* disables SysTick */
    void disable();

    /* returns the previous value */
    uint32_t store_timer_value(uint32_t value);
  }
}
