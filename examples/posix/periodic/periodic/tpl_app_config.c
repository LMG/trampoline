/**
 * @file tpl_app_config.c
 *
 * @section desc File description
 *
 * OS data structure generated from application only_one_periodic_task
 * Automatically generated by goil on Sun Dec 13 17:02:07 2015
 * from root OIL file periodic.oil
 *
 * @section copyright Copyright
 *
 * Trampoline OS
 *
 * Trampoline is copyright (c) IRCCyN 2005-2007
 * Trampoline is protected by the French intellectual property law.
 *
 * This software is distributed under the Lesser GNU Public Licence
 *
 * @section infos File informations
 *
 * $Date$
 * $Rev$
 * $Author$
 * $URL$
 */

#include "tpl_app_config.h"

#include "tpl_os_internal_types.h"
#include "tpl_machine.h"
#include "tpl_os_interrupt.h"
#include "tpl_os_interrupt_kernel.h"
#include "tpl_os_alarm_kernel.h"
#include "tpl_os_alarm.h"
#include "tpl_os_resource_kernel.h"
#include "tpl_os_resource.h"
#include "tpl_os_event_kernel.h"
#include "tpl_os_event.h"
#include "tpl_os_action.h"
#include "tpl_os_kernel.h"
#include "tpl_os_definitions.h"


/*=============================================================================
 * Application Modes tables for tasks, alarms and schedule tables
 * Application Modes masks are as follow:
 * std = 1
 */

CONST(tpl_application_mode, OS_CONST) std = 0; /* mask = 1 */
CONST(tpl_application_mode, OS_CONST) OSDEFAULTAPPMODE = 0;
CONST(tpl_appmode_mask, OS_CONST) tpl_task_app_mode[TASK_COUNT] = {
  0 /* task my_periodic_task :  */ ,
  0 /* task stop :  */ 
};

CONST(tpl_appmode_mask, OS_CONST) tpl_alarm_app_mode[ALARM_COUNT] = {
  1 /* alarm one_second : std */ ,
  1 /* alarm stopper : std */ 
};

#define API_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

/*=============================================================================
 * Declaration of processes IDs
 */

/* Task my_periodic_task identifier */
#define my_periodic_task_id 0
CONST(TaskType, AUTOMATIC) my_periodic_task = my_periodic_task_id;

/* Task stop identifier */
#define stop_id 1
CONST(TaskType, AUTOMATIC) stop = stop_id;

/*=============================================================================
 * Declaration of Alarm IDs
 */

/* Alarm one_second identifier */
#define one_second_id 0
CONST(AlarmType, AUTOMATIC) one_second = one_second_id;

/* Alarm stopper identifier */
#define stopper_id 1
CONST(AlarmType, AUTOMATIC) stopper = stopper_id;

#define API_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"
CONSTP2VAR(tpl_resource, AUTOMATIC, OS_APPL_DATA)
tpl_resource_table[RESOURCE_COUNT] = {
  &res_sched_rez_desc  
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


/*=============================================================================
 * Definition and initialization of Counters related defines and structures
 */
/*-----------------------------------------------------------------------------
 * Counter SystemCounter descriptor
 */

#define API_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

CONST(tpl_tick, OS_CONST) OSTICKSPERBASE = 1;
CONST(tpl_tick, OS_CONST) OSMAXALLOWEDVALUE = 32767;
CONST(tpl_tick, OS_CONST) OSMINCYCLE = 1;

#define API_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

VAR(tpl_counter, OS_VAR) SystemCounter_counter_desc = {
  /* ticks per base       */  1,
  /* max allowed value    */  32767,
  /* minimum cycle        */  1,
  /* current tick         */  0,
  /* current date         */  0,
#if WITH_OSAPPLICATION == YES
    /* OS Application id    */  
#endif
    /* first alarm          */  NULL_PTR,
    /* next alarm to raise  */  NULL_PTR
};

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

#include "tpl_os_kernel.h"          /* tpl_schedule */
#include "tpl_os_timeobj_kernel.h"  /* tpl_counter_tick */
#include "tpl_machine_interface.h"  /* tpl_switch_context_from_it */

#define OS_START_SEC_CODE
#include "tpl_memmap.h"
FUNC(tpl_bool, OS_CODE) tpl_call_counter_tick()
{
  tpl_counter_tick(&SystemCounter_counter_desc);

  if (tpl_kern.need_schedule)
  {
    tpl_schedule_from_running();
    LOCAL_SWITCH_CONTEXT()
  }

  return TRUE;
}

#define OS_STOP_SEC_CODE
#include "tpl_memmap.h"

/*=============================================================================
 * Descriptor(s) of the idle task(s)
 */
/*-----------------------------------------------------------------------------
 * Task IDLE_TASK descriptor
 */
#define OS_START_SEC_CODE
#include "tpl_memmap.h"
/*
 * Task IDLE_TASK function prototype
 */

FUNC(void, OS_APPL_CODE) idle_function(void);
#define OS_STOP_SEC_CODE
#include "tpl_memmap.h"

/*-----------------------------------------------------------------------------
 * Target specific structures
 */
/*
 * Task IDLE_TASK stack
 */
#define APP_Task_IDLE_TASK_START_SEC_STACK
#include "tpl_memmap.h"
tpl_stack_word IDLE_TASK_stack_zone[IDLE_STACK_SIZE/sizeof(tpl_stack_word)];
#define APP_Task_IDLE_TASK_STOP_SEC_STACK
#include "tpl_memmap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"
struct TPL_STACK IDLE_TASK_stack = {IDLE_TASK_stack_zone, IDLE_STACK_SIZE};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

#define IDLE_TASK_STACK &IDLE_TASK_stack

/*
 * Task IDLE_TASK context
 */
#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
struct TPL_CONTEXT IDLE_TASK_context;
#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

#define IDLE_TASK_CONTEXT &IDLE_TASK_context





/*
  No timing protection
 */

#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


/*
 * Static descriptor of task IDLE_TASK
 */
CONST(tpl_proc_static, OS_CONST) IDLE_TASK_task_stat_desc = {
  /* context                  */  IDLE_TASK_CONTEXT,
  /* stack                    */  IDLE_TASK_STACK,
  /* entry point (function)   */  idle_function,
  /* internal ressource       */  NULL,
  /* task id                  */  IDLE_TASK_ID,
#if WITH_OSAPPLICATION == YES
  /* OS application id        */  
#endif
  /* task base priority       */  0,
  /* max activation count     */  1,
  /* task type                */  TASK_BASIC,
#if WITH_AUTOSAR_TIMING_PROTECTION == YES

  /* execution budget */        0,
  /* timeframe        */        0, 
  /* pointer to the timing
     protection descriptor    */ NULL

#endif
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
/*
 * Dynamic descriptor of task IDLE_TASK
 */
VAR(tpl_proc, OS_VAR) IDLE_TASK_task_desc = {
  /* resources                      */  NULL,
#if WITH_OSAPPLICATION == YES
  /* if > 0 the process is trusted  */  0,    
#endif /* WITH_OSAPPLICATION */
  /* activate count                 */  0,
  /* task priority                  */  0,
  /* task state                     */  SUSPENDED
};

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"


/*=============================================================================
 * Definition and initialization of Task related defines and structures
 */
/*-----------------------------------------------------------------------------
 * Task my_periodic_task descriptor
 */
#define APP_Task_my_periodic_task_START_SEC_CODE

#include "tpl_memmap.h"
/*
 * Task my_periodic_task function prototype
 */

FUNC(void, OS_APPL_CODE) my_periodic_task_function(void);
#define APP_Task_my_periodic_task_STOP_SEC_CODE

#include "tpl_memmap.h"

/*-----------------------------------------------------------------------------
 * Target specific structures
 */
/*
 * Task my_periodic_task stack
 */
#define APP_Task_my_periodic_task_START_SEC_STACK
#include "tpl_memmap.h"
tpl_stack_word my_periodic_task_stack_zone[32768/sizeof(tpl_stack_word)];
#define APP_Task_my_periodic_task_STOP_SEC_STACK
#include "tpl_memmap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"
struct TPL_STACK my_periodic_task_stack = {my_periodic_task_stack_zone, 32768};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

#define my_periodic_task_STACK &my_periodic_task_stack

/*
 * Task my_periodic_task context
 */
#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
struct TPL_CONTEXT my_periodic_task_context;
#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

#define my_periodic_task_CONTEXT &my_periodic_task_context





/*
  No timing protection
 */

#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


/*
 * Static descriptor of task my_periodic_task
 */
CONST(tpl_proc_static, OS_CONST) my_periodic_task_task_stat_desc = {
  /* context                  */  my_periodic_task_CONTEXT,
  /* stack                    */  my_periodic_task_STACK,
  /* entry point (function)   */  my_periodic_task_function,
  /* internal ressource       */  NULL,
  /* task id                  */  my_periodic_task_id,
#if WITH_OSAPPLICATION == YES
  /* OS application id        */  
#endif
  /* task base priority       */  1,
  /* max activation count     */  1,
  /* task type                */  TASK_BASIC,
#if WITH_AUTOSAR_TIMING_PROTECTION == YES

  /* execution budget */        0,
  /* timeframe        */        0, 
  /* pointer to the timing
     protection descriptor    */ NULL

#endif
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
/*
 * Dynamic descriptor of task my_periodic_task
 */
VAR(tpl_proc, OS_VAR) my_periodic_task_task_desc = {
  /* resources                      */  NULL,
#if WITH_OSAPPLICATION == YES
  /* if > 0 the process is trusted  */  0,    
#endif /* WITH_OSAPPLICATION */
  /* activate count                 */  0,
  /* task priority                  */  1,
  /* task state                     */  SUSPENDED
};

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

/*-----------------------------------------------------------------------------
 * Task stop descriptor
 */
#define APP_Task_stop_START_SEC_CODE

#include "tpl_memmap.h"
/*
 * Task stop function prototype
 */

FUNC(void, OS_APPL_CODE) stop_function(void);
#define APP_Task_stop_STOP_SEC_CODE

#include "tpl_memmap.h"

/*-----------------------------------------------------------------------------
 * Target specific structures
 */
/*
 * Task stop stack
 */
#define APP_Task_stop_START_SEC_STACK
#include "tpl_memmap.h"
tpl_stack_word stop_stack_zone[32768/sizeof(tpl_stack_word)];
#define APP_Task_stop_STOP_SEC_STACK
#include "tpl_memmap.h"

#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"
struct TPL_STACK stop_stack = {stop_stack_zone, 32768};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

#define stop_STACK &stop_stack

/*
 * Task stop context
 */
#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
struct TPL_CONTEXT stop_context;
#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

#define stop_CONTEXT &stop_context





/*
  No timing protection
 */

#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


/*
 * Static descriptor of task stop
 */
CONST(tpl_proc_static, OS_CONST) stop_task_stat_desc = {
  /* context                  */  stop_CONTEXT,
  /* stack                    */  stop_STACK,
  /* entry point (function)   */  stop_function,
  /* internal ressource       */  NULL,
  /* task id                  */  stop_id,
#if WITH_OSAPPLICATION == YES
  /* OS application id        */  
#endif
  /* task base priority       */  2,
  /* max activation count     */  1,
  /* task type                */  TASK_BASIC,
#if WITH_AUTOSAR_TIMING_PROTECTION == YES

  /* execution budget */        0,
  /* timeframe        */        0, 
  /* pointer to the timing
     protection descriptor    */ NULL

#endif
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"
/*
 * Dynamic descriptor of task stop
 */
VAR(tpl_proc, OS_VAR) stop_task_desc = {
  /* resources                      */  NULL,
#if WITH_OSAPPLICATION == YES
  /* if > 0 the process is trusted  */  0,    
#endif /* WITH_OSAPPLICATION */
  /* activate count                 */  0,
  /* task priority                  */  2,
  /* task state                     */  SUSPENDED
};

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"


#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"
/*=============================================================================
 * Definition and initialization of process tables (tasks and isrs)
 */
CONSTP2CONST(tpl_proc_static, AUTOMATIC, OS_APPL_DATA)
tpl_stat_proc_table[TASK_COUNT+ISR_COUNT+1] = {
  &my_periodic_task_task_stat_desc,
  &stop_task_stat_desc,
  &IDLE_TASK_task_stat_desc
};

CONSTP2VAR(tpl_proc, AUTOMATIC, OS_APPL_DATA)
tpl_dyn_proc_table[TASK_COUNT+ISR_COUNT+1] = {
  &my_periodic_task_task_desc,
  &stop_task_desc,
  &IDLE_TASK_task_desc
};



#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


/*=============================================================================
 * Definition and initialization of Alarm related structures
 */
/*-----------------------------------------------------------------------------
 * Alarm one_second descriptor
 *
 * This alarm does the activation of task my_periodic_task
 */


#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

CONST(tpl_task_activation_action, OS_CONST) one_second_action = {
  {
    /* action function  */  tpl_action_activate_task
  },
  /* task id            */  my_periodic_task_id
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"
 
#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

CONST(tpl_alarm_static, OS_CONST) one_second_static = {
  {
    /* pointer to counter           */  &SystemCounter_counter_desc,
    /* pointer to the expiration    */  tpl_raise_alarm
#if (WITH_TRACE == YES)
    /* id of the alarm for tracing  */  , one_second_id
#endif
#if WITH_OSAPPLICATION == YES
    /* OS application id            */  , 
#endif
  },
  /* action of the alarm  */  (tpl_action *)&one_second_action
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

VAR(tpl_time_obj, OS_VAR) one_second_alarm_desc = {
    /* pointer to the static part   */  (tpl_time_obj_static *)&one_second_static,
    /* next alarm                   */  NULL,
    /* prev alarm                   */  NULL,
    /* cycle                        */  100,
    /* date                         */  100,
    /* State of the alarm           */  ALARM_AUTOSTART
};

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

/*-----------------------------------------------------------------------------
 * Alarm stopper descriptor
 *
 * This alarm does the activation of task stop
 */


#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

CONST(tpl_task_activation_action, OS_CONST) stopper_action = {
  {
    /* action function  */  tpl_action_activate_task
  },
  /* task id            */  stop_id
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"
 
#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

CONST(tpl_alarm_static, OS_CONST) stopper_static = {
  {
    /* pointer to counter           */  &SystemCounter_counter_desc,
    /* pointer to the expiration    */  tpl_raise_alarm
#if (WITH_TRACE == YES)
    /* id of the alarm for tracing  */  , stopper_id
#endif
#if WITH_OSAPPLICATION == YES
    /* OS application id            */  , 
#endif
  },
  /* action of the alarm  */  (tpl_action *)&stopper_action
};

#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

VAR(tpl_time_obj, OS_VAR) stopper_alarm_desc = {
    /* pointer to the static part   */  (tpl_time_obj_static *)&stopper_static,
    /* next alarm                   */  NULL,
    /* prev alarm                   */  NULL,
    /* cycle                        */  0,
    /* date                         */  1000,
    /* State of the alarm           */  ALARM_AUTOSTART
};

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"


#define OS_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"
CONSTP2VAR(tpl_time_obj, AUTOMATIC, OS_APPL_DATA)
  tpl_alarm_table[ALARM_COUNT] = {
  &one_second_alarm_desc,
  &stopper_alarm_desc
};
#define OS_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"

/*=============================================================================
 * Declaration of flags functions
 */
/* $FLAGSFUNCTIONS$ */

/*=============================================================================
 * Definition and initialization of Ready List structures
 */
#define OS_START_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

VAR(tpl_heap_entry, OS_VAR) tpl_ready_list[4];
VAR(tpl_rank_count, OS_VAR) tpl_tail_for_prio[4] = {
  0,
  0,
  0
};

#define OS_STOP_SEC_VAR_UNSPECIFIED
#include "tpl_memmap.h"

/**
 * @internal
 *
 * tpl_kern gather informations on the current executing object and
 * the previous one
 */
VAR(tpl_kern_state, OS_VAR) tpl_kern =
{
  NULL,                      /* no running task static descriptor   */
  &IDLE_TASK_task_stat_desc, /* elected task to run is idle task    */
  NULL,                      /* no running task dynamic descriptor  */
  &IDLE_TASK_task_desc,      /* elected task to run is idle task    */
  INVALID_PROC_ID,           /* no running task so no ID            */
  INVALID_PROC_ID,           /* idle task has no ID                 */
  NO_NEED_SWITCH,            /* no context switch needed at start   */
  FALSE,                     /* no schedule needed at start         */
#if WITH_MEMORY_PROTECTION == YES
  1,                         /* at early system startup, we run in  */
                             /*  kernel mode, so in trusted mode    */
#endif /* WITH_MEMORY_PROTECTION */
};

#define API_START_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"
CONSTP2CONST(char, AUTOMATIC, OS_APPL_DATA) proc_name_table[TASK_COUNT + ISR_COUNT + 1] = {

  "my_periodic_task",
  "stop",
  "*idle*"
};
#define API_STOP_SEC_CONST_UNSPECIFIED
#include "tpl_memmap.h"


/* End of file tpl_app_config.c */
