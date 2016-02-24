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
#include "tpl_control_model.h"

int current_node[TASK_COUNT] = {0};

FUNC(void, OS_CODE) compute_NEFT(
  void* param1,
  void* param2,
  void* param3,
  CONST(int, AUTOMATIC) service_id)
{
  if(tpl_kern.elected_id >= 0 && tpl_kern.elected_id < TASK_COUNT)//if it's not the IDLE task
  {
    //We compute the EFT for all outgoing transitions of the new state
    int post;
    for(post=0; post<MAX_POSTS_NUM; post++)
    {
      transition *outgoing_transition = &node[tpl_kern.elected_id][current_node[tpl_kern.elected_id]][post];
      DOW_DO(printf("Old NEFT = %u\n", (unsigned int)outgoing_transition->eft);)

      outgoing_transition->eft = tpl_get_tptimer() + outgoing_transition->lower_bound;
      if(outgoing_transition->upper_bound != -1)//-1 means infinite
      {
        outgoing_transition->lft = tpl_get_tptimer() + outgoing_transition->upper_bound;
      }
      else
      {
        outgoing_transition->lft = -1; //hopefully this is the max value of tpl_time
      }
      DOW_DO(printf("New NEFT = %u, (sc = %d, post=%d)\n",
        (unsigned int)outgoing_transition->eft, 
        outgoing_transition->sc, 
        post);)
    }
  }

}

FUNC(void, OS_CODE) slow_task(
  void* param1,
  void* param2,
  void* param3,
  CONST(int, AUTOMATIC) service_id)
{
  if(tpl_kern.elected_id >= 0 && tpl_kern.elected_id < TASK_COUNT) {
    transition* outgoing_transition = get_outgoing_transition(
      param1,
      param2,
      param3,
      service_id);

    DOW_DO(printf("Slowing task %d from %u to %u, (sc = %d, %d)\n",
      tpl_kern.elected_id,
      (unsigned int)tpl_get_tptimer(),
      (unsigned int)outgoing_transition->eft,
      service_id,
      current_node[tpl_kern.elected_id]);)
    while ( tpl_get_tptimer() < outgoing_transition->eft );
    DOW_DO(printf("Slowed task %d to %u > %u (%d)\n",
      tpl_kern.elected_id,
      (unsigned int)tpl_get_tptimer(),
      outgoing_transition->eft,
      tpl_get_tptimer() < outgoing_transition->eft );)

    //then update the model
    update_model(outgoing_transition);
  }
}

FUNC(void, OS_CODE) update_model(
  transition* outgoing_transition)
{
  if(tpl_kern.elected_id >= 0 && tpl_kern.elected_id < TASK_COUNT) {
    current_node[tpl_kern.elected_id] = outgoing_transition->target;
  }
}

FUNC(transition*, OS_CODE) get_outgoing_transition(
  void* param1,
  void* param2,
  void* param3,
  CONST(int, AUTOMATIC) service_id)
{
  DOW_DO(printf("Received transition %d from task %d at time %u\n",
    service_id, 
    tpl_kern.elected_id, 
    (unsigned int)tpl_get_tptimer());)
  
  int post;
  transition *outgoing_transition = NULL;
  if(tpl_kern.elected_id >= 0 && tpl_kern.elected_id < TASK_COUNT) {
    for(post = 0; post<MAX_POSTS_NUM; post++)
    {
      outgoing_transition = &(node[tpl_kern.elected_id][current_node[tpl_kern.elected_id]][post]);

      tpl_time current_time = tpl_get_tptimer();
      DOW_DO(printf("Testing transition %d to state %d\n tpl_kern.elected_id: %d, current_node[tpl_kern.elected_id] %d\n sc %d, id %d\n eft %d, lft %d, time %d\n",
        post,
        outgoing_transition->target,
        tpl_kern.elected_id,
        current_node[tpl_kern.elected_id],
        outgoing_transition->sc,
        service_id,
        outgoing_transition->eft,
        outgoing_transition->lft,
        current_time);)
      if(outgoing_transition->sc == service_id &&
       /*outgoing_transition->param1 == param1 &&* They are at NULL in the model for now, so don't test.
         outgoing_transition->param2 == param2 &&*
         outgoing_transition->param3 == param3 &&*/
         outgoing_transition->eft - outgoing_transition->lower_bound <= current_time && //TODO: pretty stupid to have to do that. Maybe only save firing time in "compute NEFT" and compute actual time on the fly
        (outgoing_transition->lft >= current_time ||
         outgoing_transition->lft == -1))
      {
        DOW_DO(printf("Taking it\n");)
        break;
      }
    }
  }
  return outgoing_transition;
}
