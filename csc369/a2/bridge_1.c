#include "bridge_1.h"

/* The value MAX_THREADS in bridge.h limits how many concurrent threads
 * we can have at one time, but we may want to push more traffic through
 * the bridge than this value.  The "more_vehicles" semaphore allows our
 * main thread to wait until some vehicle is done before creating another
 * vehicle thread. (Think about why this is better than using pthread_join)  
 */
sem_t more_vehicles;


/* Current number of vehicles waiting to cross the bridge
   in northbound and southbound directions. */
int numWaitingSouth = 0, numWaitingNorth = 0;

/* Current number of vehicles crossing the bridge in
   northbound and southbound directions. */
int numOnBridgeSouth = 0, numOnBridgeNorth = 0;

/* Maximum observed number of vehicles waiting to cross
   the bridge in northbound and southbound directions. */
int maxWaitingNorth = 0, maxWaitingSouth = 0;

/* Maximum number of vehicles on the bridge at one time. */
int maxOnBridge = 0;

/* Mutex and condition variables that govern access
   to the bridge. */
pthread_mutex_t bridgeLock;
pthread_cond_t bridgeCond = PTHREAD_COND_INITIALIZER;


/* Mutex and condition variables that are used for updating
   the current number of vehicle threads. */
pthread_mutex_t threadCounterLock;
pthread_cond_t threadCounterCond = PTHREAD_COND_INITIALIZER;

/* Counter of current vehicle threads */
int threadCounter = 0;


/*******************************************************************************
 * function driveCar() - used by a Car thread, simply calls crossBridge() 
 * passing in the current state as a parameter.
 ******************************************************************************/
void *driveCar(void *threadarg){
	crossBridge((vstate_t *)threadarg);
}

/*******************************************************************************
 * function driveTruck() - used by a Truck thread, simply calls crossBridge() 
 * passing in the current state as a parameter.
 ******************************************************************************/
void *driveTruck(void *threadarg){
  crossBridge((vstate_t *)threadarg);
}

/*******************************************************************************
 * function driveBicycle() - used by a Bicycle thread, simply calls crossBridge() 
 * passing in the current state as a parameter.
 ******************************************************************************/
void *rideBicycle(void *threadarg){
  crossBridge((vstate_t *)threadarg);
}


/*******************************************************************************
 * function crossBridge() - models the crossing of a bridge by a vehicle, takes
 * the current vehicle state as a parameter.
 ******************************************************************************/
void crossBridge(vstate_t *mystate){

 
	/* Acquire bridge lock. */
	pthread_mutex_lock(&bridgeLock);
		
	/* If heading North */
	if(mystate->dir == Northbound){
		
		/* Update the number of waiting northbound vehicles
		   and the maximum number of waiting northbound vehicles. */
		if(++numWaitingNorth > maxWaitingNorth){
			maxWaitingNorth = numWaitingNorth;
		}
		
		/* Block while vehicles are travelling in the opposite direction. */
		while(numOnBridgeSouth > 0){
			pthread_cond_wait(&bridgeCond, &bridgeLock);
		}
		
		/* Can now proceed onto the bridge, so update the number of waiting
		   northbound vehicles, the number of vehicles crossing the bridge
			 northbound, and the max number of vehicles on the bridge at once. */
		numWaitingNorth--;
		
		if(++numOnBridgeNorth > maxOnBridge){
			maxOnBridge = numOnBridgeNorth;
		}
		
	
	/* If heading South */
	}else{
		
		/* Update the number of waiting southbound vehicles
		   and the maximum number of waiting southbound vehicles. */
		if(++numWaitingSouth > maxWaitingSouth){
			maxWaitingSouth = numWaitingSouth;
		}

		/* Block while vehicles are travelling in the opposite direction. */
		while(numOnBridgeNorth > 0){
			pthread_cond_wait(&bridgeCond, &bridgeLock);
		}
		
		/* Can now proceed onto the bridge, so update the number of waiting
		   southbound vehicles, the number of vehicles crossing the bridge
			 southbound, and the max number of vehicles on the bridge at once. */
		numWaitingSouth--;
		
		if(++numOnBridgeSouth > maxOnBridge){
			maxOnBridge = numOnBridgeSouth;
		}
	}
 
	/* Release the bridge lock. */
	pthread_mutex_unlock(&bridgeLock);

	sleep(1);  /* pass some time on the bridge */

	/* Acquire the bridge lock. */
 	pthread_mutex_lock(&bridgeLock);

	/* If heading north. */
	if(mystate->dir == Northbound){
		
		/* If the last vehicle currently travelling northbound
		   notify the southbound vehicles that they can now
			 cross the bridge. */
		if(--numOnBridgeNorth == 0){
			pthread_cond_broadcast(&bridgeCond);
		}
	/* If heading south. */	
	}else{
		
		/* If the last vehicle currently travelling southbound
		   notify the northbound vehicles that they can now
			 cross the bridge. */
		if(--numOnBridgeSouth == 0){
			pthread_cond_broadcast(&bridgeCond);
		}
	
	}
  
	/* Release the bridge lock. */
	pthread_mutex_unlock(&bridgeLock);

	 /* cleanup and exit */
  free(mystate);
  sem_post(&more_vehicles);

	pthread_mutex_lock(&threadCounterLock);
	if(--threadCounter == 0){
		pthread_cond_signal(&threadCounterCond);
	}
	pthread_mutex_unlock(&threadCounterLock);
	pthread_exit((void *)0);
}

int main(int argc, char *argv[]) {
  int count = 0;  /* used to assign id's to vehicles */
  FILE *input;    /* input file pointer */
  char line[80];  /* a complete line from the file */
  char rest[10];  /* the rest of the line after first char */
  char vtype;     /* first character of the line, the vehicle type */
  int wait_time;  /* time to wait before continuing to process file */
  
  vstate_t *vehicle_state; /* vehicle state to pass to thread */
  pthread_t vehicle_th;    /* thread id of newly created vehicle thread */
  
	/* Initialize bridge lock. */
	pthread_mutex_init(&bridgeLock,NULL);

	/* Initialize thread counter lock. */
	pthread_mutex_init(&threadCounterLock, NULL);

  /* Check for simple usage errors */
  if(argc != 2) {
    printf("Usage: bridge <input file>\n");
    exit(1);
  }

  /* Try to open input file */
  if ( (input = fopen(argv[1],"r")) == 0) {
    snprintf(line, 80, "Error opening file %s",argv[1]);
    perror(line);
    exit(1);
  }

  /* Initialize the semaphore to the maximum number of vehicle threads
   * we are allowed */
  sem_init(&more_vehicles, 0, MAX_THREADS);

  /* Process the input file */
  while(!feof(input)) {

    if (fgets(line, 80, input) == NULL) {
      continue;
    }

    /* Make sure we are allowed to create another thread */
    sem_wait(&more_vehicles);

    vehicle_state = (vstate_t *) malloc(sizeof(vstate_t));
    vehicle_state->id = count;

    sscanf(line,"%c %s\n",&vtype,rest);
    
    switch(vtype) {
    case 'W':
      wait_time = atoi(rest);
      sleep(wait_time);
			sem_post(&more_vehicles);
      free(vehicle_state);
      continue;
    case 'C':
      vehicle_state->type = Car;
      vehicle_state->dir = (rest[0] == 'N' ? Northbound : Southbound);
      pthread_create(&vehicle_th, NULL, driveCar, (void *)vehicle_state);
      break;
    case 'T':
      vehicle_state->type = Truck;
      vehicle_state->dir = (rest[0] == 'N' ? Northbound : Southbound);
      pthread_create(&vehicle_th, NULL, driveTruck, (void *)vehicle_state);
      break;
    case 'B':
      vehicle_state->type = Bicycle;
      vehicle_state->dir = (rest[0] == 'N' ? Northbound : Southbound);
      pthread_create(&vehicle_th, NULL, rideBicycle, (void *)vehicle_state);
      break;
    default:
      printf("Error in input file: %s\n",line);
      exit(1);
    }

    /* We don't want to wait for a specific vehicle to finish,
     * so just detach the threads
     */
    pthread_detach(vehicle_th);
    count++;

		/* Acquire thread counter lock and increase number of
		   vehicle threads. */
		pthread_mutex_lock(&threadCounterLock);
		threadCounter++;
		pthread_mutex_unlock(&threadCounterLock);
  }


	/* Block while vehicle threads are still running.*/
	pthread_mutex_lock(&threadCounterLock);
	while(threadCounter > 0){
		pthread_cond_wait(&threadCounterCond, &threadCounterLock);
	}
	pthread_mutex_unlock(&threadCounterLock);
	
  /* When vehicle threads are done print the maximum number of vehicles
	   waiting to cross in the northbound and southbound directions as well
		 as the maximum number of vehicles on the bridge at one time. */
	printf("Done!\n");
  printf("Maximum number of vehicles waiting to cross in Northbound direction was %d\n", maxWaitingNorth);
  printf("Maximum number of vehicles waiting to cross in Southbound direction was %d\n", maxWaitingSouth);
  printf("Maximum number of vehicles on the bridge at one time was %d\n", maxOnBridge);
	exit(0);

}
