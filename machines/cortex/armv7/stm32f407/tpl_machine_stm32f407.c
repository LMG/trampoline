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

#include "tpl_as_timing_protec.h"

#define OS_START_SEC_CODE
#include "tpl_memmap.h"
extern volatile uint32 tpl_time_counter;

FUNC(void, OS_CODE) tpl_set_systick_timer()
{
	if (SysTick_Config(SystemCoreClock / 1000))
	{
		while(1);
	}
}

FUNC(uint32, OS_CODE) tpl_get_tptimer()
{
  /*
   * SystemCoreClock is the CPU frequency in Hz
   * SYST_CVR is the SysTick Current Value Refister (the SysTick is a decrementer)
   * tpl_time_counter is incremented each time the systick reaches 0, so every 1ms
   */
   return (((SystemCoreClock / 1000) - 1) - SysTick->VAL) / 42 + 4000 * tpl_time_counter;
}
#define OS_STOP_SEC_CODE
#include "tpl_memmap.h"

