/**
 * @file tpl_os_it_kernel.c
 *
 * @section desc File description
 *
 * Trampoline interrupt service routines management
 *
 * @section copyright Copyright
 *
 * Trampoline RTOS
 *
 * Trampoline is copyright (c) CNRS, University of Nantes, Ecole Centrale de Nantes
 * Trampoline is protected by the French intellectual property law.
 *
 * This software is distributed under the GNU Public Licence V2.
 * Check the LICENSE file in the root directory of Trampoline
 *
 * @section infos File informations
 *
 * $Date$
 * $Rev$
 * $Author$
 * $URL$
 */

#include "tpl_os_definitions.h"
#include "tpl_os_kernel.h"
#include "tpl_os_error.h"
#include "tpl_os_errorhook.h"
#include "tpl_machine_interface.h"
#include "tpl_trace.h"
#include "tpl_os_interrupt_kernel.h"

#if defined(__unix__) || defined(__APPLE__)
#include <assert.h>
#endif

#if WITH_STACK_MONITORING == YES
#include "tpl_as_stack_monitor.h"
#endif /* WITH_AUTOSAR_STACK_MONITORING */

#if WITH_AUTOSAR == YES
#include "tpl_as_isr_kernel.h"
#endif

#if WITH_AUTOSAR_TIMING_PROTECTION == YES
#include "tpl_as_protec_hook.h"
#include "tpl_as_timing_protec.h"
#endif


#define OS_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "tpl_memmap.h"
/*
 * the following variableѕ should not be initialized at definition,
 * or Memmap section is not the right one
 */
volatile VAR(uint32, OS_VAR) tpl_locking_depth = 0;
VAR(tpl_bool, OS_VAR) tpl_user_task_lock = FALSE;
VAR(uint32, OS_VAR) tpl_cpt_user_task_lock_All = 0;
VAR(uint32, OS_VAR) tpl_cpt_user_task_lock_OS = 0;
VAR(uint32, OS_VAR) tpl_cpt_os_task_lock = 0;

#if ISR_COUNT > 0
STATIC VAR(sint32, OS_VAR) tpl_it_nesting =  0;
#endif

#define OS_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "tpl_memmap.h"

#define OS_START_SEC_CODE
#include "tpl_memmap.h"
FUNC(tpl_bool, OS_CODE) tpl_get_interrupt_lock_status(void)
{
    VAR(tpl_bool, AUTOMATIC) result;

    if ((TRUE == tpl_user_task_lock) ||
        (tpl_cpt_user_task_lock_OS > 0) ||
        (tpl_cpt_user_task_lock_All > 0))
    {
        result = TRUE;
    }
    else
    {
        result = FALSE;
    }

    return result;
}

FUNC(void, OS_CODE) tpl_reset_interrupt_lock_status(void)
{
	tpl_user_task_lock = FALSE;

	tpl_cpt_user_task_lock_All = 0;
	tpl_cpt_user_task_lock_OS = 0;

	tpl_locking_depth = tpl_cpt_os_task_lock;
}

/**
 * TODO: document this
 */
FUNC(void, OS_CODE) tpl_suspend_all_interrupts_service(void)
{
  tpl_disable_interrupts();

  tpl_locking_depth++;

  tpl_cpt_user_task_lock_All++;

}

/**
 * TODO: document this
 */
FUNC(void, OS_CODE) tpl_resume_all_interrupts_service(void)
{
  #if defined(__unix__) || defined(__APPLE__)
	assert( tpl_locking_depth >= 0 );
  #endif

	if( tpl_cpt_user_task_lock_All != 0 )
	{
		tpl_locking_depth--;

		tpl_cpt_user_task_lock_All--;

		if( tpl_locking_depth == 0)
		{
			tpl_enable_interrupts();
		}
	}

}

/**
 * TODO: document this
 */
FUNC(void, OS_CODE) tpl_disable_all_interrupts_service(void)
{
  if ((0 == tpl_cpt_user_task_lock_All) &&
      (0 == tpl_cpt_user_task_lock_OS))
  {
	  tpl_disable_interrupts();

	  tpl_user_task_lock = TRUE;
  }
}

/**
 * TODO: document this
 */
FUNC(void, OS_CODE) tpl_enable_all_interrupts_service(void)
{
	if (tpl_user_task_lock != FALSE)
	{
		tpl_user_task_lock = FALSE;

		tpl_enable_interrupts();
	}
}

/**
 * TODO: document this
 */
FUNC(void, OS_CODE) tpl_suspend_os_interrupts_service(void)
{
  tpl_disable_interrupts();

  tpl_locking_depth++;

  tpl_cpt_user_task_lock_OS++;

}

/**
 * TODO: document this
 */
FUNC(void, OS_CODE) tpl_resume_os_interrupts_service(void)
{
  #if defined(__unix__) || defined(__APPLE__)
	assert(tpl_locking_depth >= 0);
  #endif

	if (tpl_cpt_user_task_lock_OS != 0)
	{
		tpl_locking_depth--;

		tpl_cpt_user_task_lock_OS--;

		if (0 == tpl_locking_depth)
		{
			tpl_enable_interrupts();
		}
	}
}

/*
 * tpl_terminate_isr2_service
 *
 * While this function is not part of the OSEK API, it is used
 * to terminate an ISR2 routine
 */
FUNC(tpl_status, OS_CODE) tpl_terminate_isr2_service(void)
{
  GET_CURRENT_CORE_ID(core_id)

  /* init the error to no error */
  VAR(tpl_status, AUTOMATIC) result = E_OK;

  CHECK_INTERRUPT_LOCK(result)

  /* check we are at the ISR2 level */
  CHECK_ISR2_CALL_LEVEL_ERROR(result)
  /* check the ISR2 does not own a resource */
  CHECK_RUNNING_OWNS_REZ_ERROR(core_id, result)

#if ISR_COUNT > 0
  IF_NO_EXTENDED_ERROR(result)

  /* the activate count is decreased */
  TPL_KERN(core_id).running->activate_count--;

  /* terminate the running ISR */
  tpl_terminate();
  /* start the highest priority process */
  tpl_start();
  /* process switching should occur */
  TPL_KERN(core_id).need_switch = NEED_SWITCH;

  LOCAL_SWITCH_CONTEXT_NOSAVE(a_core_id)

  IF_NO_EXTENDED_ERROR_END()
#endif

  PROCESS_ERROR(result)

  return result;
}

FUNC(void, OS_CODE) tpl_null_it(P2CONST(void, OS_APPL_DATA, AUTOMATIC) foo)
{
  /* empty function */
}

#define OS_STOP_SEC_CODE
#include "tpl_memmap.h"

#if ISR_COUNT > 0

#define OS_START_SEC_CODE
#include "tpl_memmap.h"

/*
 */
STATIC FUNC(void, OS_CODE) tpl_activate_isr(
  CONST(tpl_isr_id, AUTOMATIC) isr_id)
{
  CONSTP2VAR(tpl_proc, AUTOMATIC, OS_APPL_DATA) isr =
    tpl_dyn_proc_table[isr_id];
  /*
   * MISRA RULE 33 VIOLATION: the right statement does
   * not need to be executed if the first test fails
   */
  if ((isr->activate_count < tpl_stat_proc_table[isr_id]->max_activate_count)
#if WITH_AUTOSAR == YES
      && (tpl_is_isr2_enabled(isr_id))
#endif
      )
  {
#if WITH_AUTOSAR_TIMING_PROTECTION == YES
    /* a new instance is about to be activated: we need the agreement
       of the timing protection mechanism                                */
    if (TRUE == tpl_tp_on_activate_or_release(isr_id))
    {
#endif
      if (isr->activate_count == 0)
      {
        /*  check the isr is in the PROC_SUSPENDED state before moving it        */
        if (isr->state == (tpl_proc_state)PROC_SUSPENDED)
        {
          isr->state = (tpl_proc_state)PROC_READY_AND_NEW;
        }
      }
      /*  put it in the list  */
      TRACE_ISR_ACTIVATE(isr_id);
      tpl_put_new_proc(isr_id);
      /*  inc the isr activation count. When the isr will terminate
          it will dec this count and if not zero it will be reactivated   */
      isr->activate_count++;
#if WITH_AUTOSAR_TIMING_PROTECTION == YES
    }
    else /* timing protection forbids the activation of the instance   */
    {
      /*  OS466: If an attempt is made to activate a task before the
          end of an OsTaskTimeFrame then the Operating System module
          shall not perform the activation AND
          shall call the ProtectionHook() with E_OS_PROTECTION_ARRIVAL. */
      tpl_call_protection_hook(E_OS_PROTECTION_ARRIVAL);
    }
#endif
  }
}

/*
 * The central interrupt handler is called by the interrupt handler
 * with the id of the interrupt (usually its priority) as parameter
 * tpl_central_interrupt_handler saves the context of the interrupted
 * task / interrupt handler, switches to the context of the handler
 * and calls the handler
 */
FUNC(void, OS_CODE) tpl_central_interrupt_handler(
  CONST(uint16, AUTOMATIC) isr_id)
{
  P2CONST(tpl_isr_static, AUTOMATIC, OS_APPL_DATA) isr;

#if WITH_STACK_MONITORING == YES
    tpl_check_stack((tpl_proc_id)tpl_kern.running_id);
#endif /* WITH_AUTOSAR_STACK_MONITORING */

  /*
   * Is there a handler for this id ?
   * ie the id has been counted in the table and there
   * is a tpl_isr available
   */
#if WITH_OS_EXTENDED == YES
  if ((isr_id >= TASK_COUNT) && (isr_id < (TASK_COUNT + ISR_COUNT)))
  {
#endif
    tpl_it_nesting++;

    isr = tpl_isr_stat_table[isr_id - TASK_COUNT];

    if (isr != NULL)
    {
      if ((isr->next) == NULL)
      {
        /* Only one handler for this id. run the handler */
        tpl_activate_isr(isr->isr_id);
      }
      else
      {
        /* look for the handler */
        while (isr != NULL)
        {
          if (isr->helper() == TRUE)
          {
            /* activate the handler */
            tpl_activate_isr(isr->isr_id);

          }
          isr = isr->next;
        }
      }
    }

    tpl_it_nesting--;

    if (tpl_it_nesting == 0)
    {
      tpl_schedule_from_running();
      LOCAL_SWITCH_CONTEXT(a_core_id)
    }
#if WITH_OS_EXTENDED == YES
  }
#endif
}


/*
 * The central interrupt handler is called by the interrupt handler
 * with the id of the interrupt (usually its priority) as parameter
 * tpl_central_interrupt_handler saves the context of the interrupted
 * task / interrupt handler, switches to the context of the handler
 * and calls the handler. VP2 version
 */
FUNC(void, OS_CODE) tpl_central_interrupt_handler_2(P2CONST(void, OS_APPL_DATA, AUTOMATIC) isr_id)
{
    uint32 tmp;
    tmp = (uint32)isr_id;
    tpl_central_interrupt_handler(tmp);
}

#define OS_STOP_SEC_CODE
#include "tpl_memmap.h"

#endif /* ISR_COUNT */

/* End of file tpl_os_it_kernel.c */
