/*******************************************************************************
 * CSC 354
 * Assignment 1
 * By: Sergei Iakhnin
 * #: 981 843 420
 * Task 3
 ******************************************************************************/
#include "csim.h"

#define MAX_TIME 1000
#define IATM 1
#define SVTM1 0.7
#define SVTM2 0.9

EVENT done;		/*pointer for event */
EVENT done2;
BOX queue_box, queue_box2;
BOX service_box, service_box2;

FACILITY f1;
FACILITY f2;

void sim();
void cust();
void init();

void sim(){
	
	create("sim");
	
	/* Initialize simulation. */
	init();
	
	/* For the duration of the simulation generate customers with 
	   exponential inter-arrival times with mean IATM. */
	while(simtime() < 1000){
		hold(exponential(IATM));
		cust();
		
	}
	wait(done);
	
	/* Print reports. */
	printf("Server 1, expected average delay in queue of a customer: %f\n",
	table_mean(box_time_table(queue_box)));
	printf("Server 1, expected time-average number of customers in queue: %f\n",
	qtable_mean(box_number_qtable(queue_box)));
	printf("Server 1, expected utilization: %f\n\n",
	qtable_mean(box_number_qtable(service_box)));
	
	printf("Server 2, expected average delay in queue of a customer: %f\n",
	table_mean(box_time_table(queue_box2)));
	printf("Server 2, expected time-average number of customers in queue: %f\n",
	qtable_mean(box_number_qtable(queue_box2)));
	printf("Server 2, expected utilization: %f\n\n",
	qtable_mean(box_number_qtable(service_box2)));

}

void init(){
	/* Initialize two facilities to represent servers. */
	f1 = facility("facility 1");
	f2 = facility("facility 2");
	
	/* Initialize an event to signal simulation completion */
	done = event("done");
	
	/* Initialize some boxes for bookkeeping. */
	queue_box = box("queue");
	queue_box2 = box("queue 2");
	service_box = box("in service");
	service_box2 = box("in service 2");
}

void cust(){
  /* If simulation hasn't ended yet. */
	if(clock < MAX_TIME){
		TIME t1, t2, t3, t4;

		create("cust");

		/* Customer enters queue at facility 1. */
		t1 = enter_box(queue_box);
		reserve(f1);

		/* Customer exits queue at facility 1. */
		exit_box(queue_box, t1);

		/* Customer enters service at facility 1. */
		t2 = enter_box(service_box);

		/* Service customer at facility 1. */
		hold(exponential(SVTM1));

		/* Customer finished service at facility 1. */
		exit_box(service_box, t2);

		/* Customer enters queue at facility 2. */
		t3 = enter_box(queue_box2);

		/* Request facility 2 without releasing facility 1. */	
		reserve(f2);

		/* Customer exits queue at facility 2. */
		exit_box(queue_box2, t3);

		/* Customer enters service at facility 2. */
		t4 = enter_box(service_box2);

		/* Customer has been granted facility 2 so can now release facility 1. */
		release(f1);

		/* Service customer at facility 2. */
		hold(exponential(SVTM2));

		release(f2);

		/* Customer finished service at facility 2. */
		exit_box(service_box2, t4);

		/* Check if time to terminate. */
		if(clock >= 1000){
			set(done);
		}
	/* If time to end simulation, signal the main process. */
	}else{
		set(done);
	}
}
