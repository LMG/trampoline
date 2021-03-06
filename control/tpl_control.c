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

#include "tpl_control.h"
#include "tpl_service_ids.h"

VAR(tpl_time, AUTOMATIC) NEFT[TASK_NUM] = {0, 0};//NEFT is set to 0 for all tasks
int act = 0;
int current_node[TASK_NUM] = {0};
transition node[TASK_NUM][MAX_NODES_NUM][MAX_POSTS_NUM] = 
{
  //RcvTask model
  {
    //Node 0
    {
      //Outgoing transition 0
      {1, START_TASK, 0, -1, 0, -1, ""},
    },
    //Node 1
    {
      //Outgoing transition 0
      {2, OSServiceId_WaitEvent, 0, -1, 0, -1, ""}, 
      {0, OSServiceId_TerminateTask, 0, -1, 0, -1, ""},//when using redundancy
    },
    //Node 2
    {
      //Outgoing transition 0
      {0, OSServiceId_TerminateTask, 0, -1, 0, -1, ""}, 
    }
  },
  //SndTask model
  {
    //Node 0
    {
      //Outgoing transition 0
      {1, START_TASK, 0, -1, 0, -1, ""},
    },
    //Node 1
    {
      //Outgoing transition 0
      {2, OSServiceId_SetEvent, 62, -1, 0, -1, ""}, 
    },
    //Node 2
    {
      //Outgoing transition 0
      {0, OSServiceId_TerminateTask, 0, -1, 0, -1, ""}, 
    }
  },
};

//TODO: Dummy function to replace tpl_get_local_current_date()
unsigned int tpl_get_local_current_date(){
  return (unsigned int)42;
}

FUNC(StatusType, OS_CODE) compute_NEFT(
  void* param1,
  void* param2,
  void* param3,
  CONST(int, AUTOMATIC) service_id)
{
  DOW_DO(printf("Received transition %d from task %d at time %u\n", service_id, tpl_kern.running_id, (unsigned int)tpl_get_local_current_date());)
  
  if(tpl_kern.running_id < TASK_NUM)//if it's not the IDLE task
  {
    //we update the state (in the model) of the calling task and compute the EFT for all outgoing transitions of the new state
    int post;
    for(post=0; post<MAX_POSTS_NUM; post++)
    {
      transition *outgoing_transition = &node[tpl_kern.running_id][current_node[tpl_kern.running_id]][post];
      DOW_DO(printf("Testing transition %d to state %d\n tpl_kern.running_id: %d, current_node[tpl_kern.running_id] %d\n", post, outgoing_transition->target, tpl_kern.running_id, current_node[tpl_kern.running_id]);)
      if(outgoing_transition->sc == service_id && tpl_get_local_current_date() >= outgoing_transition->eft && (tpl_get_local_current_date() <= outgoing_transition->lft || outgoing_transition->lft == -1))
      {
        DOW_DO(printf("Taking transition %d to state %d\n tpl_kern.running_id: %d, current_node[tpl_kern.running_id] %d\n", post, outgoing_transition->target, tpl_kern.running_id, current_node[tpl_kern.running_id]);)
        current_node[tpl_kern.running_id] = outgoing_transition->target;
        break;
      }
    }
    for(post=0; post<MAX_POSTS_NUM; post++)
    {
      transition *outgoing_transition = &node[tpl_kern.running_id][current_node[tpl_kern.running_id]][post];
      DOW_DO(printf("Old NEFT = %u\n", (unsigned int)outgoing_transition->eft);)
      outgoing_transition->eft = tpl_get_local_current_date() + outgoing_transition->lower_bound;
      if(outgoing_transition->upper_bound != -1)//-1 means infinite
      {
        outgoing_transition->lft = tpl_get_local_current_date() + outgoing_transition->upper_bound;
      }
      else
      {
        outgoing_transition->lft = -1; //hopefully this is the max value of tpl_time
      }
      DOW_DO(printf("New NEFT = %u, (sc = %d, post=%d)\n", (unsigned int)outgoing_transition->eft, outgoing_transition->sc, post);)
    }
  }

}

FUNC(StatusType, OS_CODE) slow_task(
  void* param1,
  void* param2,
  void* param3,
  CONST(int, AUTOMATIC) service_id)
{
  int post;
  for(post=0; post<MAX_POSTS_NUM; post++)
  {
    if(node[tpl_kern.running_id][current_node[tpl_kern.running_id]][post].sc == service_id)
    {
      unsigned int eft = node[tpl_kern.running_id][current_node[tpl_kern.running_id]][post].eft;
      DOW_DO(printf("Slowing task %d from %u to %u, (sc = %d, post = %d, %d)\n", tpl_kern.running_id, (unsigned int)tpl_get_local_current_date(), (unsigned int)eft, service_id, post, current_node[tpl_kern.running_id]);)
      while ( tpl_get_local_current_date() < eft );
      DOW_DO(printf("Slowed task %d to %u > %u (%d)\n", tpl_kern.running_id, (unsigned int)tpl_get_local_current_date(), eft,  tpl_get_local_current_date() < eft );)
      break;//exit the for loop
    }
  }
}
