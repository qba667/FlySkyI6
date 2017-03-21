#ifndef _DELAY_H_
#define _DELAY_H_
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */
#include "board.h"
#include "clock_config.h"

volatile uint32_t msTicks;

void SysTick_Handler(void)
{
	msTicks++;
}
__inline __attribute__((section (".TestCODE"))) static void _delay_ms(uint32_t del)
{
	uint32_t curTicks;
	curTicks = msTicks;
	while ((msTicks - curTicks) < del)
	{
		__asm("wfi");
	}
}

__attribute__((section (".TestCODE"))) void BOARD_SysTick(void)		// Setup Systick!
{
	if (SysTick_Config(SystemCoreClock / 1000))
	{
		while (1);
	}
}
__attribute__((section (".TestCODE"))) void delay_us(int a1)
{
  int count = 768 * a1;
  while (count){
	  count--;
  }
}

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _DELAY_H_ */
