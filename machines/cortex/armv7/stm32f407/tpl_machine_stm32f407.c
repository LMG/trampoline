/**
 * @file tpl_machine_stm32f407.c
 *
 * @section descr File description
 *
 * Trampoline processor dependant memory protection for MPC551x mpu.
 *
 * @section copyright Copyright
 *
 * Trampoline OS
 *
 *  Trampoline is copyright (c) IRCCyN 2005-2009
 *  Autosar extension is copyright (c) IRCCyN and ESEO 2007-2009
 *  Trampoline and its Autosar extension are protected by the
 *  French intellectual property law.
 *
 *  This software is distributed under a dual licencing scheme
 *  1 - The Lesser GNU Public Licence v2 (LGPLv2)
 *  2 - The BSD Licence
 *
 * @section infos File informations
 *
 * $Date$
 * $Rev$
 * $Author$
 * $URL$
 */

#include "tpl_compiler.h"
#include "tpl_os_std_types.h"
#include "tpl_machine.h"

#if WITH_AUTOSAR_TIMING_PROTECTION == YES
/* files needed for TIM and NVIC stuff */
#include "stm32f4xx_tim.h"

#include "tpl_as_timing_protec.h"

#endif /* WITH_AUTOSAR_TIMING_PROTECTION */


#define OS_START_SEC_CODE
#include "tpl_memmap.h"

FUNC(void, OS_CODE) tpl_set_systick_timer()
{
	if (SysTick_Config(SystemCoreClock / 1000))
	{
		while(1);
	}
}

#if WITH_AUTOSAR_TIMING_PROTECTION == YES
extern volatile uint32 tpl_time_counter;

FUNC(void, OS_CODE) tpl_set_tpwatchdog (CONST(tpl_time, AUTOMATIC) delay)
{
 /*
   * Program the timer
   *
   *
   */
  NVIC_InitTypeDef NVIC_InitStructure;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

  /* TIM2 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  /* Enable the TIM2 gloabal Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Time base configuration */
  /* Prescaler so the counter counts at sysclock freq*/
  TIM_TimeBaseStructure.TIM_Prescaler = 0;

  /* Time set to delay value: needs to be expressed in ticks (prescaler = 0) */
  TIM_TimeBaseStructure.TIM_Period = delay;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  TIM_SelectOnePulseMode(TIM2, TIM_OPMode_Single);

  TIM_Cmd(TIM2, ENABLE);
}

/*
 * TIM2 interrupt handler
 * It is called by the interrupt vector
 */
FUNC(void, OS_CODE) TIM2_IRQ_Handler(void)
{
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
  {
    tpl_watchdog_expiration(); //This function is defined in ~/autosar/tpl_as_timing_protec.c
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
  }
}

FUNC(void, OS_CODE) tpl_cancel_tpwatchdog (void)
{
  TIM_Cmd(TIM2, DISABLE);
}

FUNC(uint32, OS_CODE) tpl_get_tptimer()
{
  /*
   * SystemCoreClock is the CPU frequency in Hz
   * SysTick->VAL is the SysTick Current Value Register (the SysTick is a decrementer)
   * tpl_time_counter is incremented each time the systick reaches 0, so every 1ms
   *
   */
   return (((SystemCoreClock / 1000) - 1) - SysTick->VAL) + (SystemCoreClock /1000 ) * tpl_time_counter;
}

/* necessaray for ST libraries */
FUNC(void, OS_CODE) assert_failed(uint8_t* file, uint32_t line)
{

}

#define OS_STOP_SEC_CODE
#include "tpl_memmap.h"

#endif /* WITH_AUTOSAR_TIMING_PROTECTION */

