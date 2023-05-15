#include <cstdio>
#include <cstdint>
#include "HAL/HalSysTick.h"

int main() {
  printf("started\n");
    
  HAL::SysTick::enter_configuration_lock();
  HAL::SysTick::set_source(HAL::SysTick::SYSTICK_SOURCE_SYSTEM_CLOCK);
  HAL::SysTick::enable_interrupts();
  HAL::SysTick::store_timer_value(20000000);
  HAL::SysTick::enable();
  HAL::SysTick::leave_configuration_lock();

  while (1);
}
