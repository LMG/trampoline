/*
 * Trampoline OS
 *
 * Trampoline is copyright (c) IRCCyN 2005+
 * Trampoline is protected by the French intellectual property law.
 *
 * This software is distributed under the Lesser GNU Public Licence
 *
 * Trampoline control solution
 *
 * $Date$
 * $Rev$
 * $Author$
 * $URL$
 */

#ifndef __TPL_CONTROL__
#define __TPL_CONTROL__

#include "tpl_os_definitions.h"
#include "tpl_os_custom_types.h"
#include "tpl_os_types.h"

#include "tpl_os_event.h"
#include "tpl_os_event_kernel.h"

#include "tpl_dow.h"

//maximum number of transitions after a given node
#define MAX_POSTS_NUM 2
//maximum number of nodes in a given task
#define MAX_NODES_NUM 3
//size of id data
#define DATA_SIZE 10

//the start of the task is not a system call, so we use -1
#define START_TASK -1

//Tasks models
typedef struct {
  int target; //target node, [0, MAX_NODES_NUM]
  int sc; //system call as defined in tpl_service_ids.h
  int lower_bound;  //lower bound computed offline
  int upper_bound;  //lower bound computed offline
  tpl_time eft; //earliest firing time computed online
  tpl_time lft; //latest firing time computed online
  void* param1;
  void* param2;
  void* param3;
} transition;

FUNC(void, OS_CODE) compute_NEFT(
  void* param1,
  void* param2,
  void* param3,
  CONST(int, AUTOMATIC) service_id);
FUNC(void, OS_CODE) slow_task(
  void* param1,
  void* param2,
  void* param3,
  CONST(int, AUTOMATIC) service_id);
FUNC(void, OS_CODE) update_model(
  transition* outgoing_transition);
FUNC(transition*, OS_CODE) get_outgoing_transition(
  void* param1,
  void* param2,
  void* param3,
  CONST(int, AUTOMATIC) service_id);

extern transition node[TASK_COUNT][MAX_NODES_NUM][MAX_POSTS_NUM];//for example: outgoing transition 2 of node 1 of task 0: node[0][1][2]
extern int current_node[TASK_COUNT];

#endif
/* __TPL_CONTROL__ */
