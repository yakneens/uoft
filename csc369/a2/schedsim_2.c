#include "schedsim_2.h"
#include <string.h>


//Define length of time slice
#define TIME_SLICE 100

//Global for assigning process ids
int pid = 0;
/********************************************************************/
/***** Utility routines for event list *****/


/* Inserts new event into list, sorted by start_time */
void enqueueEventByStartTime(event_list_e **list, event_list_e *new_event) {

  event_list_e *prev, *curr;

  if (*list == NULL) {
    *list = new_event;
  } else {
    prev = NULL;
    curr = *list;
    while (curr && curr->start_time <= new_event->start_time) {
      prev = curr;
      curr = curr->next;
    }
    /* post:  curr points to first element with time > new time (if any)
     * prev points to previous element, or null
     */
    new_event->next = curr; /* might be NULL */
    if (prev)
      prev->next = new_event;
    else
      *list = new_event;
  }

}

/* Returns the event at the head of the list, which should be the
 * one with the smallest start_time
 */
event_list_e *dequeueEvent(event_list_e **list) {

  event_list_e *first = *list;
  if (*list)
    *list = (*list)->next;
  
  return first;

}

/********************************************************************/
/***** Utility routines for work lists *****/

/* Add a new work item at the end of the list */
void enqueueWorkListItem(work_list_e **list, work_list_e *new_item) {
  work_list_e *prev, *curr;
  
  if (*list == NULL) {
		*list = new_item;
  } else {
    /* Find end of list */
    prev = *list;
    curr = prev->next;
    while (curr) {
      prev = curr;
      curr = curr->next;
    }
    prev->next = new_item;
  }
  new_item->next = NULL;
}

/* Add a new work item at the beginning of the list */
void pushWorkListItem(work_list_e **list, work_list_e *new_item) {
	if(new_item != NULL){
		new_item->next = (*list);
		*list = new_item;
	}
  
}

/* Return the first work item in the list */
work_list_e *dequeueWorkListItem(work_list_e **list) {
  work_list_e *first = *list;

  if(*list)
    *list = (*list)->next;

  return first;
}


/* Inserts new event into list, sorted by start_time */
void enqueueProcessByPriority(PD_t **list, PD_t *new_process){

  PD_t *prev, *curr;

  if (*list == NULL) {
    *list = new_process;
  } else {
    prev = NULL;
    curr = *list;
    while (curr != NULL && curr->priority >= new_process->priority) {
      prev = curr;
      curr = curr->next;
		}
    /* post:  curr points to first element with time > new time (if any)
     * prev points to previous element, or null
     */
    new_process->next = curr; /* might be NULL */
    if (prev)
      prev->next = new_process;
    else
      *list = new_process;
  }

}

/* Returns the event at the head of the list, which should be the
 * one with the largest priority.
 */
PD_t *dequeueProcess(PD_t **list){

  PD_t *first = *list;
  if (*list)
    *list = (*list)->next;
  
  if(first != NULL){
		first->next = NULL;	
	}
	
	return first;

}

/* Removes and returns the process with process id = pid from list
 * If no such process exists in list returns NULL
 */
PD_t *removeProcessByID(PD_t **list, int pid){

  PD_t *cur = *list, *prev = NULL;
  
	while(cur != NULL){
		
		if(cur->pid == pid){
			if(prev == NULL){
				(*list) = cur->next;
			}else{
				prev->next = cur->next;
			}

			break;
		}else{
			prev = cur;
			cur = cur->next;
		}
	}
  
	if(cur != NULL){
		cur->next = NULL;
	}
  
	return cur;

}

/* Returns the process with process id = pid from list
 * If no such process exists in list returns NULL
 */
PD_t *getProcessByID(PD_t **list, int pid){

  PD_t *cur = *list;
  
	while(cur != NULL){
		
		if(cur->pid == pid){
			return cur;
			break;
		}else{
			cur = cur->next;
		}
	}
  
  return cur;

}

/* Add a new process at the end of the list */
void enqueueProcess(PD_t **list, PD_t *new_process){
  PD_t *prev, *curr;
  
  if (*list == NULL) {
		*list = new_process;
  } else {
    /* Find end of list */
    prev = *list;
    curr = prev->next;
    while (curr) {
      prev = curr;
      curr = curr->next;
    }
    prev->next = new_process;
  }
  new_process->next = NULL;
}

/* Determines whether list is empty */
int isEmpty(PD_t **list){
	return (*list) == NULL;
}
/********************************************************************/

/* Read input file and initialize process descriptors and events.
 * Create a new process descriptor for each line of the input file
 * and store the descriptor in the list of all processes (you will need
 * to write the list manipulation routines for lists of processes
 * yourself).
 * Also create a "ProcessBegin" event for each process, using the
 * process arrival time as the "start_time" for the event and add it
 * to the event list
 */
void ProcessInput(FILE *input, event_list_e **events, PD_t **processes) {
	burst_t type = CPU;
	work_list_e *newItem;
		
	char buf[81];
	char* start_time;
	char* priority;
	char* burst;
	
	//Read through the input file
	while(!feof(input)){
		event_list_e *new_event = malloc(sizeof(event_list_e));
		PD_t *theProcess = malloc(sizeof(PD_t));
		work_list_e *myWorkList = NULL;
	
		//get a line of input
		fgets(buf, 81, input);
		
		if(feof(input)){
			break;
		}
		
		//get current process start time from input
		start_time = strtok(buf, " ");

		//get current process priority from input
		priority = strtok(NULL, " ");
		

		//initialize priority, state, run_time, wait_time, blocked_time
		// and pid fields of the current process
		theProcess->priority = atoi(priority);
		theProcess->state = NEW;
		theProcess->run_time = 0;
		theProcess->wait_time = 0;
		theProcess->blocked_time = 0;
		theProcess->last_ready_time = 0;
		theProcess->pid = pid++;
		
		//Store the burst lengths associated with this process
		//in a new work list.
		while(burst = strtok(NULL, " ")){
			if(!strcmp(burst, "\n")){
				break;
			}
			
			newItem = malloc(sizeof(work_list_e));

			newItem->type = type;

			//alternate between CPU and IO bursts.
			if(type == CPU){
				type = IO;
			}else{
				type = CPU;
			}

			
			//initialize current burst duration
			newItem->duration = atoi(burst);

			//add current burst to work list.
			enqueueWorkListItem(&myWorkList, newItem);
			
		}
	
		//Add the list of bursts to the current process
		theProcess->bursts = myWorkList;
		theProcess->next = NULL;
	
		//Add the current process to the list of all processes.
		enqueueProcess(processes, theProcess);
	
		
		//Record a new ProcessBegin event for this process
		//and add it to the event queue.
		new_event->start_time = atoi(start_time);
		new_event->pid = theProcess->pid;
		new_event->type = ProcessBegin;
		new_event->next = NULL;

		enqueueEventByStartTime(events, new_event);
	}


}



/* Run scheduling simulation.  You will need to write handlers for
 * each type of event.  In many cases, handling an event will involve
 * adding a new event to the list (for instance, a TimerExpire event
 * should create a new TimerExpire event one timer interval in the future.
 * Similarly, an IOBegin event will create an IODone event at a later time
 * based on the length of the I/O burst.
 *
 * You will also need to implement the function to choose the next
 * process to run at scheduling points.  You will want to create 
 * additional lists of process descriptors to represent the ready
 * queue and the processes blocked for I/O.  
 *
 * When there are no more events to process, the simulation should
 * return.  Verify that all the processes are in the "DONE" state
 * at this point.  
 */
void RunSimulation(event_list_e **events, PD_t **processes) {
  
  int time = 0;
	event_list_e *curEvent;
	
	//Process structures for the ready queue, io queue
	//Currently running process and current process
	PD_t *readyQueue = NULL, *ioQueue = NULL, *current = NULL, *running = NULL;
	work_list_e *thisBurst, *newItem;
	
	//Retreive each event in the queue one by one
	while((curEvent = dequeueEvent(events)) != 0){
		
		//Advance the time
		time = curEvent->start_time;
		
		//Detect event type
		switch(curEvent->type){
			
		//ProcessBegin event
		case ProcessBegin:
			
			//Get process for this event from list of all processes
			current = removeProcessByID(processes, curEvent->pid);
			
			//Process is now in ready state.
			current->state = READY;
			
			//If process can be scheduled immediately
			if(isEmpty(&readyQueue) && running == NULL){
				
				//Get current burst
				thisBurst = dequeueWorkListItem(&(current->bursts));
				
				//Schedule process for execution
				current->state = RUNNING;

				//If the duration of this burst exceeds the duration
				//of the time slice
				if(thisBurst->duration > TIME_SLICE){
					
					//Create and schedule a new TimerExpire event
					event_list_e *newEvent = malloc(sizeof(event_list_e));
					newItem = malloc(sizeof(work_list_e));

					newEvent->pid = current->pid;
					newEvent->type = TimerExpire;
					newEvent->start_time = time + TIME_SLICE;
					newEvent->next = NULL;
					enqueueEventByStartTime(events, newEvent);

					//Record and add the part of the burst that did
					//not complete within the time slice
					newItem->type = CPU;
					newItem->duration = thisBurst->duration - TIME_SLICE;

					pushWorkListItem(&(current->bursts), newItem);
					
					//Increase run time for this process by length
					//of time slice
					current->run_time += TIME_SLICE;
				
				//If the burst can complete within the time slice
				}else{
					//Increase process run time by length of current CPU burst
					current->run_time += thisBurst->duration;
					
					//Create and enqueue a new IOBegin event for this process.
					if(current->bursts != NULL){
						event_list_e *newEvent = malloc(sizeof(event_list_e));
						newItem = malloc(sizeof(work_list_e));
						newEvent->pid = current->pid;
						newEvent->type = IOBegin;
						newEvent->start_time = time + thisBurst->duration;
						newEvent->next = NULL;
						enqueueEventByStartTime(events, newEvent);
					//Create and enqueue a new ProcessEnd event for this process
					}else{
						event_list_e *newEvent = malloc(sizeof(event_list_e));
						newEvent->pid = current->pid;
						newEvent->type = ProcessEnd;
						newEvent->start_time = time + thisBurst->duration;
						newEvent->next = NULL;
						enqueueEventByStartTime(events, newEvent);

					}
				}
				
				
				//Set the current process to be the running process.
				running = current;
				
				
			//If process cannot be scheduled immediately
			//Send process to the ready queue
			//Record when process entered ready state.
			}else{
				current->last_ready_time = time;
				enqueueProcess(&readyQueue, current);
			}
			break;
		//ProcessEnd event
		case ProcessEnd:
			
			//If ending process is currently running
			if(running != NULL && running->pid == curEvent->pid){
				current = running;
				running = NULL;
			//Otherwise retreive process from io queue
			}else{
				current = removeProcessByID(&ioQueue, curEvent->pid);
			}

			//The process is now complete.
			current->state = DONE;

			//Add process back to the list of all processes
			//for tallying of results.
			enqueueProcess(processes, current);
			break;
		
		//IOBegin event
		case IOBegin:
			
			//set the process state to blocked
			current = running;
			current->state = BLOCKED;
			running = NULL;
			
			//Retreive and record the length of the current IO burst
			thisBurst = dequeueWorkListItem(&(current->bursts));
			current->blocked_time += thisBurst->duration;

			//If more bursts left schedule a IODone event
			//for this process
			if(current->bursts != NULL){
				event_list_e *newEvent = malloc(sizeof(event_list_e));
				newEvent->pid = current->pid;
				newEvent->type = IODone;
				newEvent->start_time = time + thisBurst->duration;
				newEvent->next = NULL;
				enqueueEventByStartTime(events, newEvent);
			//If no bursts left schedule a ProcessEnd event
			//for this process
			}else{
				event_list_e *newEvent = malloc(sizeof(event_list_e));
				newEvent->pid = current->pid;
				newEvent->type = ProcessEnd;
				newEvent->start_time = time + thisBurst->duration;
				newEvent->next = NULL;
				enqueueEventByStartTime(events, newEvent);

			}

			//place process in io queue
			enqueueProcess(&ioQueue, current);

			//If processes waiting in ready queue
			if(!isEmpty(&readyQueue)){
				
				//Retreive next process from ready queue
				current = dequeueProcess(&readyQueue);
				
				//Get current burst
				thisBurst = dequeueWorkListItem(&(current->bursts));
				
				//Schedule process for execution
				current->state = RUNNING;
				
				//Record process wait time
				current->wait_time += time - current->last_ready_time;
				running = current;

				//If the duration of this burst exceeds the duration
				//of the time slice
				if(thisBurst->duration > TIME_SLICE){
					
					//Create and schedule a new TimerExpire event
					event_list_e *newEvent = malloc(sizeof(event_list_e));
					newItem = malloc(sizeof(work_list_e));

					newEvent->pid = current->pid;
					newEvent->type = TimerExpire;
					newEvent->start_time = time + TIME_SLICE;
					newEvent->next = NULL;
					enqueueEventByStartTime(events, newEvent);

					//Record and add the part of the burst that did
					//not complete within the time slice
					newItem->type = CPU;
					newItem->duration = thisBurst->duration - TIME_SLICE;

					pushWorkListItem(&(current->bursts), newItem);
					
					//Increase run time for this process by length
					//of time slice
					current->run_time += TIME_SLICE;
				
				//If the burst can complete within the time slice
				}else{
					//Increase process run time by length of current CPU burst
					current->run_time += thisBurst->duration;
					
					//Create and enqueue a new IOBegin event for this process.
					if(current->bursts != NULL){
						event_list_e *newEvent = malloc(sizeof(event_list_e));
						newItem = malloc(sizeof(work_list_e));
						newEvent->pid = current->pid;
						newEvent->type = IOBegin;
						newEvent->start_time = time + thisBurst->duration;
						newEvent->next = NULL;
						enqueueEventByStartTime(events, newEvent);
					//Create and enqueue a new ProcessEnd event for this process
					}else{
						event_list_e *newEvent = malloc(sizeof(event_list_e));
						newEvent->pid = current->pid;
						newEvent->type = ProcessEnd;
						newEvent->start_time = time + thisBurst->duration;
						newEvent->next = NULL;
						enqueueEventByStartTime(events, newEvent);

					}
				}
			}
			break;
		//IODone event
		case IODone:
			//Remove process from io queue.
			current = removeProcessByID(&ioQueue, curEvent->pid);
			
			//Process is now in ready state.
			current->state = READY;
			
			//Record time process entered ready state
			current->last_ready_time = time;
			
			//Place process at the back of the ready queue
			enqueueProcess(&readyQueue, current);
			break;
		
		//TimerExpire event
		case TimerExpire:
			current = running;
			running = NULL;
			
			//Process is now in ready state
			current->state = READY;
			
			
			//If no other processes to run
			if(isEmpty(&readyQueue) && running == NULL){

				//Get current burst
				thisBurst = dequeueWorkListItem(&(current->bursts));
				
				//Schedule process for execution
				current->state = RUNNING;

				//If the duration of this burst exceeds the duration
				//of the time slice
				if(thisBurst->duration > TIME_SLICE){
					
					//Create and schedule a new TimerExpire event
					event_list_e *newEvent = malloc(sizeof(event_list_e));
					newItem = malloc(sizeof(work_list_e));

					newEvent->pid = current->pid;
					newEvent->type = TimerExpire;
					newEvent->start_time = time + TIME_SLICE;
					newEvent->next = NULL;
					enqueueEventByStartTime(events, newEvent);

					//Record and add the part of the burst that did
					//not complete within the time slice
					newItem->type = CPU;
					newItem->duration = thisBurst->duration - TIME_SLICE;

					pushWorkListItem(&(current->bursts), newItem);
					
					//Increase run time for this process by length
					//of time slice
					current->run_time += TIME_SLICE;
				
				//If the burst can complete within the time slice
				}else{
					//Increase process run time by length of current CPU burst
					current->run_time += thisBurst->duration;
					
					//Create and enqueue a new IOBegin event for this process.
					if(current->bursts != NULL){
						event_list_e *newEvent = malloc(sizeof(event_list_e));
						newItem = malloc(sizeof(work_list_e));
						newEvent->pid = current->pid;
						newEvent->type = IOBegin;
						newEvent->start_time = time + thisBurst->duration;
						newEvent->next = NULL;
						enqueueEventByStartTime(events, newEvent);
					//Create and enqueue a new ProcessEnd event for this process
					}else{
						event_list_e *newEvent = malloc(sizeof(event_list_e));
						newEvent->pid = current->pid;
						newEvent->type = ProcessEnd;
						newEvent->start_time = time + thisBurst->duration;
						newEvent->next = NULL;
						enqueueEventByStartTime(events, newEvent);

					}
				}
				
				

				running = current;
				
				
			//If process cannot be scheduled immediately
			//Send process to the ready queue
			//Retrieve next process from ready queue and
			//schedule it for execution
			}else{
				
				//Send current process to ready queue
				current->last_ready_time = time;
				enqueueProcess(&readyQueue, current);
			
				//Retreive next process for execution
				current = dequeueProcess(&readyQueue);

				//Get current burst
				thisBurst = dequeueWorkListItem(&(current->bursts));
				
				//Schedule process for execution
				current->state = RUNNING;

				//If the duration of this burst exceeds the duration
				//of the time slice
				if(thisBurst->duration > TIME_SLICE){
					
					//Create and schedule a new TimerExpire event
					event_list_e *newEvent = malloc(sizeof(event_list_e));
					newItem = malloc(sizeof(work_list_e));

					newEvent->pid = current->pid;
					newEvent->type = TimerExpire;
					newEvent->start_time = time + TIME_SLICE;
					newEvent->next = NULL;
					enqueueEventByStartTime(events, newEvent);

					//Record and add the part of the burst that did
					//not complete within the time slice
					newItem->type = CPU;
					newItem->duration = thisBurst->duration - TIME_SLICE;

					pushWorkListItem(&(current->bursts), newItem);
					
					//Increase run time for this process by length
					//of time slice
					current->run_time += TIME_SLICE;
				
				//If the burst can complete within the time slice
				}else{
					//Increase process run time by length of current CPU burst
					current->run_time += thisBurst->duration;
					
					//Create and enqueue a new IOBegin event for this process.
					if(current->bursts != NULL){
						event_list_e *newEvent = malloc(sizeof(event_list_e));
						newItem = malloc(sizeof(work_list_e));
						newEvent->pid = current->pid;
						newEvent->type = IOBegin;
						newEvent->start_time = time + thisBurst->duration;
						newEvent->next = NULL;
						enqueueEventByStartTime(events, newEvent);
					//Create and enqueue a new ProcessEnd event for this process
					}else{
						event_list_e *newEvent = malloc(sizeof(event_list_e));
						newEvent->pid = current->pid;
						newEvent->type = ProcessEnd;
						newEvent->start_time = time + thisBurst->duration;
						newEvent->next = NULL;
						enqueueEventByStartTime(events, newEvent);

					}
				}
				
				

				running = current;

			}
			break;
		default:
			break;
		}
	
	}
  /* While more events
   *     Remove first event
   *     Advance time to start of event
   *     Handle event
   */

}

int main(int argc, char *argv[]) {
  int curTurnaroundTime = 0;
  int totalWaitTime = 0, totalTurnaroundTime = 0;
  int numProcesses = 0;
  double avgWaitTime = 0, avgTurnaroundTime = 0;
 
  FILE *input;
  char line[80];

  event_list_e *events = NULL;
  PD_t *all_processes = NULL;

  if (argc != 2) {
    printf("Usage: schedsim <input file>\n");
    exit(1);
  }


   /* Try to open input file */
  if ( (input = fopen(argv[1],"r")) == 0) {
    snprintf(line, 80, "Error opening file %s",argv[1]);
    perror(line);
    exit(1);
  }

  ProcessInput(input, &events, &all_processes);

  
  
  RunSimulation(&events, &all_processes);

  //Going through all the processes print out
  //wait time and turnaround time for each process.
  //Accumulate total wait time and turnaround time.
  while(all_processes != NULL){
	PD_t *thisProcess = dequeueProcess(&all_processes);
	numProcesses++;
	curTurnaroundTime = thisProcess->wait_time + thisProcess->blocked_time + thisProcess->run_time;
	printf("ProcessID: %d, Wait Time: %d, Turnaround Time: %d\n", thisProcess->pid, thisProcess->wait_time, curTurnaroundTime); 
	totalWaitTime += thisProcess->wait_time;
	totalTurnaroundTime += curTurnaroundTime;
  }
 
  //Print out average wait time and
  //average turnaround time for the processes
  if(numProcesses != 0){
	avgWaitTime = totalWaitTime / numProcesses;
	avgTurnaroundTime = totalTurnaroundTime / numProcesses;
	printf("Average Wait Time: %f, Average Turnaround Time: %f\n", avgWaitTime, avgTurnaroundTime);
  }

  //End of simulation
  return 0;

}
