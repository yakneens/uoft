/*************************
 * CSC369 - Assignment 1
 * By: Sergei Iakhnin
 * #: 981843420
 * File: measure.c
 ************************/

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/types.h>
#include "etime.h"



#define REL_ERR 0.01


/*******************************************
 * Wrapper function for the wait() system call.
 * Prints an appropriate error message and 
 * exits if the wait() call is unsuccessful.
 *******************************************/
pid_t Wait(int *status){
	
	int rvalue = 0;
	
	if((rvalue == wait(status)) < 0){
		fprintf(stderr, "Wait error: %s\n", strerror(errno));
		exit(1);
	}

	return rvalue;
	
}

/*******************************************
 * Wrapper function for the fork() system call.
 * Prints an appropriate error message and 
 * exits if the fork() call is unsuccessful.
 *******************************************/
pid_t Fork(){
	pid_t id;
	
	if((id = fork()) == -1){
		fprintf(stderr, "Fork error: %s\n", strerror(errno));
		exit(1);
	}
	
	return id;
}

/*******************************************
 * Wrapper function for the pthread_create() 
 * library function. Prints an appropriate 
 * error message and exits if the
 * pthread_create() call is unsuccessful.
 *******************************************/
void Pthread_create(pthread_t *thread, const pthread_attr_t *attr,
	     void *(*start_routine)(void *), void *arg){

	if(pthread_create(thread, attr, start_routine, arg)){
		fprintf(stderr, "Pthread_create error: %s\n", strerror(errno));
		exit(1);
	}
}

/*******************************************
 * Wrapper function for the pthread_join() 
 * library function. Prints an appropriate 
 * error message and exits if the
 * pthread_join() call is unsuccessful.
 *******************************************/
void Pthread_join(pthread_t thread, void **value_ptr){
	if(pthread_join(thread, value_ptr)){
		fprintf(stderr, "Pthread_join error: %s\n", strerror(errno));
		exit(1);
	}
}

/*******************************************
 * Wrapper function for the pthread_exit() 
 * library function. Prints an appropriate 
 * error message and exits if the
 * pthread_exit() call is unsuccessful.
 *******************************************/
void Pthread_exit(void *value_ptr){
	pthread_exit(value_ptr);
}


/*******************************************
 * Function get_delta() measures the length
 * of the ITIMER_REAL timer increment 
 * on a Linux kernel.
 *******************************************/
double get_delta(){
	double a,b;
	int i, bound = 79;

	init_etime_real();
	
	/* Set the current value of the timer to two variables. */
	a = get_etime_real();
	b = a;

	/* Assign current value of the timer to b.
	   while comparing with a until the two values
           are different. */ 
	while(a == (b = get_etime_real()));
		
	
	/* The difference between values is the current
           timer increment. */
	return b - a;

}

/*******************************************
 * Function part_a() calls get_delta() 80 
 * times and prints out the average timer
 * increment. Since a typical linux CPU
 * timeslice is about 1 sec 80 iterations
 * should provide a reliable and accurate
 * estimate of the correct timer increment.
 *******************************************/
void part_a(){
	int bound = 80, i;
	double total_incr = 0;
	for(i = 0; i < bound; i++){
		total_incr += get_delta();
	}
	
	printf("The value of delta is : %f\n", total_incr / bound);
	
}

/*******************************************
 * Function part_b() computes and prints
 * the average time to create and exit a 
 * process using fork() as measured by 
 * ITIMER_PROF timer. 
 *******************************************/
void part_b(){
	
	int pid, i;
	long bound = 1;
	double start_time, end_time, threshold, delta, total_time = 0;	
	
	/* Compute timer increment. */
	delta = get_delta();
	
	/* Compute error threshold. */
	threshold = (1 / REL_ERR + 1) * delta;
		
	/* Each iteration of this loop the number
           of processes that are created is doubled 
           until an error threshold is reached. */
	while(1){

		/* Initialize timer. */
		init_etime_prof();
		
		/* Record initial time. */
		start_time = get_etime_prof();
	
		/* Iterate through bound process creations. */
		for(i = 0; i < bound; i++){	

			/* Fork a child. If in the child
                           exit. If in the parent wait for
                           a child to exit(). */
			if((pid = Fork()) == 0){
				exit(0);
			}else{
				Wait(NULL);
			}

		}

		/* Record final time. */
		end_time = get_etime_prof();
		
		/* The difference is the total time 
                   spent in user and kernel modes. */
		total_time = end_time - start_time;
		
		
		/* If total_time exceeds the threshold
                   desired results have been obtained.
                   Break out of the main loop. */
		if(total_time > threshold){break;}
		
		/* Double the number of processes
		   for the next iteration. */		
		bound *= 2;
	}
	
	/* Print the number of iterations elapsed. */
 	printf("Number of iterations is %ld\n", bound);
	
        /* Compute and print the average overhead per 
           process. */
	printf("Average overhead per process is %f\n", total_time / bound);

}


/*******************************************
 * Function handle_thread() terminates the 
 * current thread.
 *******************************************/
void *handle_thread(void *arg){
	//printf("hello");
	Pthread_exit(NULL);
}


/*******************************************
 * Function part_c() computes and prints
 * the average time to create and exit a 
 * thread using pthreads as measured by 
 * ITIMER_PROF timer. 
 *******************************************/
void part_c(){
	int pid, i;
	long bound = 1;
	double start_time, end_time, threshold, delta, total_time = 0;	
	pthread_t my_thread;

	/* Compute timer increment. */
	delta = get_delta();
	
	/* Compute error threshold. */
	threshold = (1 / REL_ERR + 1) * delta;
		
	/* Each iteration of this loop the number
           of threads that are created is doubled 
           until an error threshold is reached. */
	while(1){

		/* Initialize timer. */
		init_etime_prof();
		
		/* Record initial time. */
		start_time = get_etime_prof();
	
		/* Iterate through bound process creations. */
		for(i = 0; i < bound; i++){		
			
			/* Create and join a child thread with null attributes
                           and arguments. handle_thread is a pointer to new thread
                           entry-point function. */
			Pthread_create(&my_thread, NULL, handle_thread, (void *)NULL);
			Pthread_join(my_thread, NULL);
		}

		/* Record final time. */
		end_time = get_etime_prof();
		
		/* The difference is the total time 
                   spent in user and kernel modes. */
		total_time = end_time - start_time;
		
		
		/* If total_time exceeds the threshold
                   desired results have been obtained.
                   Break out of the main loop. */
		if(total_time > threshold){break;}
		
		/* Double the number of processes
		   for the next iteration. */		
		bound *= 2;
	}
	
	/* Print the number of iterations elapsed. */
 	printf("Number of iterations is %ld\n", bound);
	
        /* Compute and print the average overhead per 
           thread. */
	printf("Average overhead per thread is %f\n", total_time / bound);

}

/*******************************************
 * Main entry-point into the program. Parses
 * command-line options and calls the
 * appropriate handling function.
 *******************************************/
main(int argc, char *argv[]) {
    
	int c;

	opterr = 0;
	
	while( (c = getopt(argc, argv, "abc")) != EOF){
		switch(c){
		case 'a':
			part_a();
			break;
		case 'b':
			part_b();			
			break;
		case 'c':
			part_c();
			break;
		case '?':
			printf("Unrecognized option -%c\n", optopt);
			return(-1);

		}

	}
	
	
}

