#include <stdio.h>
#include <stdlib.h>

/*******************************************************************/

typedef enum event_types {
  ProcessBegin,
  ProcessEnd,
  IOBegin,
  IODone,
  TimerExpire
} event_t;

typedef struct event_list_entry {
  event_t type;
  int pid;
  int start_time;
  struct event_list_entry *next;
} event_list_e;

void enqueueEventByStartTime(event_list_e **list, event_list_e *new_event);
event_list_e *dequeueEvent(event_list_e **list);

/*******************************************************************/

typedef enum bursts {
  CPU,
  IO
} burst_t;

typedef struct work_list_entry {
  burst_t type;
  int duration;
  struct work_list_entry *next;
} work_list_e;

void enqueueWorkListItem(work_list_e **list, work_list_e *new_item);
work_list_e *dequeueWorkListItem(work_list_e **list);

/*******************************************************************/

typedef enum process_state {
  NEW,
  READY,
  RUNNING,
  BLOCKED,
  DONE
} pstate_t;

typedef struct process_descriptor {
  int pid;
  pstate_t state;
  int priority;
  int run_time;
  int wait_time;
  int blocked_time;
  int last_ready_time;
  work_list_e *bursts;
  struct process_descriptor *next;
} PD_t;


/*******************************************************************/
void enqueueProcessByPriority(PD_t**, PD_t*);
void enqueueProcess(PD_t**, PD_t*);
PD_t *dequeueProcess(PD_t**);
PD_t *removeProcessByID(PD_t**, int);
PD_t *getProcessByID(PD_t**, int);
int isEmpty(PD_t**);
