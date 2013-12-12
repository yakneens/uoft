#include "bridge_3.h"

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


int nwbs = 0, nwcs = 0, nwts = 0;
int nobbs = 0, nobcs = 0, nobts = 0;

int nwbn = 0, nwcn = 0, nwtn = 0;
int nobbn = 0, nobcn = 0, nobtn = 0;

pthread_mutex_t textLock;

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

pthread_cond_t southCond = PTHREAD_COND_INITIALIZER;
pthread_cond_t northCond = PTHREAD_COND_INITIALIZER;

pthread_cond_t northTruckCond = PTHREAD_COND_INITIALIZER;
pthread_cond_t southTruckCond = PTHREAD_COND_INITIALIZER;
pthread_cond_t northCarCond = PTHREAD_COND_INITIALIZER;
pthread_cond_t southCarCond = PTHREAD_COND_INITIALIZER;

pthread_cond_t northWaitTruckCond = PTHREAD_COND_INITIALIZER;
pthread_cond_t southWaitTruckCond = PTHREAD_COND_INITIALIZER;
pthread_cond_t northWaitCarCond = PTHREAD_COND_INITIALIZER;
pthread_cond_t southWaitCarCond = PTHREAD_COND_INITIALIZER;

/*******************************************************************************
 * function driveCar() - used by a Car thread, simply calls crossBridge() 
 * passing in the current state as a parameter.
 ******************************************************************************/
void *driveCar(void *threadarg){
	vstate_t *mystate = (vstate_t *)threadarg;

	printf("Car %d arrives to the bridge heading %s\n", mystate->id, DIR2STR(mystate->dir));
	fflush(stdout);
	
	
	/* Acquire bridge lock. */
	pthread_mutex_lock(&bridgeLock);
	
	/* If heading North */
	if(mystate->dir == Northbound){
		
		/* Update the number of waiting northbound vehicles
		   and the maximum number of waiting northbound vehicles. */
		if(++numWaitingNorth > maxWaitingNorth){
			maxWaitingNorth = numWaitingNorth;
		}
		
		nwcn++;
		
		if(numOnBridgeSouth > 0){
			/* Block while vehicles are travelling south that are either cars or trucks. */
			while(numOnBridgeSouth > 0 && (nobcs > 0 || nobts > 0)){
				
				/* There are cars crossing southbound. */
				if(nobcs > 0){
					printf("Car %d is blocked due to %d cars on the bridge crossing Southbound\n", mystate->id, nobcs);
					fflush(stdout);
					pthread_cond_wait(&northCarCond, &bridgeLock);
				
				/* There are trucks crossing southbound. */
				}else if(nobts > 0){
					printf("Car %d is blocked due to %d trucks on the bridge crossing Southbound\n", mystate->id, nobts);
					fflush(stdout);
					pthread_cond_wait(&northTruckCond, &bridgeLock);
				}
				
			}
			
		}else if(numOnBridgeSouth == 0){
			/* Block if there are vehicles waiting to cross south that are not bicycles. */
			if(numWaitingSouth > 0 && (numWaitingSouth != nwbs)){
				
				/* There are cars waiting to cross southbound. */
				if(nwcs > 0){
					printf("Car %d is blocked due to %d cars waiting to cross Southbound\n", mystate->id, nwcs);
					fflush(stdout);
					pthread_cond_wait(&northWaitCarCond, &bridgeLock);
				/* There are trucks waiting to cross southbound. */
				}else if(nwts > 0){
					printf("Car %d is blocked due to %d trucks waiting to cross Southbound\n", mystate->id, nwts);
					fflush(stdout);
					pthread_cond_wait(&northWaitTruckCond, &bridgeLock);
				}
			}
		}
		/* Can now proceed onto the bridge, so update the number of waiting
		   northbound vehicles, the number of vehicles crossing the bridge
			 northbound, and the max number of vehicles on the bridge at once. */
		numWaitingNorth--;
		
		/* When no more cars waiting to cross northbound signal to threads
			 waiting on that condition. */
		if(--nwcn == 0){
			pthread_cond_broadcast(&southWaitCarCond);
		}

		nobcn++;

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

		nwcs++;

		if(numOnBridgeNorth > 0){
			/* Block while vehicles are travelling north that are either cars or trucks. */
			while(numOnBridgeNorth > 0 && (nobcn > 0 || nobtn > 0)){
				/* There are cars corssing the bridge Northbound. */
				if(nobcn > 0){
					printf("Car %d is blocked due to %d cars on the bridge crossing Northbound\n", mystate->id, nobcn);
					fflush(stdout);
					pthread_cond_wait(&southCarCond, &bridgeLock);
				/* There are trucks crossing the bridge Southbound. */
				}else if(nobtn > 0){
					printf("Car %d is blocked due to %d trucks on the bridge crossing Northbound\n", mystate->id, nobtn);
					fflush(stdout);
					pthread_cond_wait(&southTruckCond, &bridgeLock);
				}
			}
		}else if(numOnBridgeNorth == 0){
			/* Block if there are vehicles waiting to cross northbound that are not bicycles. */
			if(numWaitingNorth > 0 && (numWaitingNorth != nwbn)){
				
				/* There are cars waiting to cross northbound. */
				if(nwcn > 0){
					printf("Car %d is blocked due to %d cars waiting to cross Northbound\n", mystate->id, nwcn);
					fflush(stdout);
					pthread_cond_wait(&southWaitCarCond, &bridgeLock);
				/* There are trucks waiting to cross northbound. */
				}else if(nwtn > 0){
					printf("Car %d is blocked due to %d trucks waiting to cross Northbound\n", mystate->id, nwtn);
					fflush(stdout);
					pthread_cond_wait(&southWaitTruckCond, &bridgeLock);
				}
			}
		}
		/* Can now proceed onto the bridge, so update the number of waiting
		   southbound vehicles, the number of vehicles crossing the bridge
			 southbound, and the max number of vehicles on the bridge at once. */
		numWaitingSouth--;
		
		/* When no more cars waiting to cross southbound signal to threads
			 waiting on that condition. */
		if(--nwcs == 0){
			pthread_cond_broadcast(&northWaitCarCond);
		}
		
		nobcs++;
		
		if(++numOnBridgeSouth > maxOnBridge){
			maxOnBridge = numOnBridgeSouth;
		}
	}
 
	/* Release the bridge lock. */
	pthread_mutex_unlock(&bridgeLock);
	printf("Car %d is crossing the bridge heading %s\n", mystate->id, DIR2STR(mystate->dir));
	fflush(stdout);
//	printState();
	sleep(1);  /* pass some time on the bridge */

	/* Acquire the bridge lock. */
 	pthread_mutex_lock(&bridgeLock);

	/* If heading north. */
	if(mystate->dir == Northbound){
		
		printf("Car %d is exiting the bridge heading %s\n", mystate->id, DIR2STR(mystate->dir));
		fflush(stdout);

		if(numWaitingSouth > 0 && --numOnBridgeNorth == 0){
			pthread_cond_broadcast(&southCond);
		}
	
		/* When no more cars are crossing the bridge northbound signal to threads
		   waiting on that condition. */
		if(--nobcn == 0){
			pthread_cond_broadcast(&southCarCond);	
		}


		/* If the last vehicle currently travelling northbound
		   notify the southbound vehicles that they can now
			 cross the bridge. */
		if(numOnBridgeNorth == 0){
			pthread_cond_broadcast(&southCond);
		}

	/* If heading south. */	
	}else{
		
		if(numWaitingNorth > 0 && --numOnBridgeSouth == 0){
			pthread_cond_broadcast(&northCond);
		}

		/* When no more cars are crossing the bridge southbound signal to threads
		   waiting on that condition. */
		if(--nobcs == 0){
			pthread_cond_broadcast(&northCarCond);	
		}

		/* If the last vehicle currently travelling southbound
		   notify the northbound vehicles that they can now
			 cross the bridge. */
		if(numOnBridgeSouth == 0){
			pthread_cond_broadcast(&northCond);
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

/*******************************************************************************
 * function driveTruck() - used by a Truck thread, simply calls crossBridge() 
 * passing in the current state as a parameter.
 ******************************************************************************/
void *driveTruck(void *threadarg){
  vstate_t *mystate = (vstate_t *)threadarg;
	
	printf("Truck %d arrives to the bridge heading %s\n", mystate->id, DIR2STR(mystate->dir));
	fflush(stdout);
		
	/* Acquire bridge lock. */
	pthread_mutex_lock(&bridgeLock);
	
	/* If heading North */
	if(mystate->dir == Northbound){
		
		/* Update the number of waiting northbound vehicles
		   and the maximum number of waiting northbound vehicles. */
		if(++numWaitingNorth > maxWaitingNorth){
			maxWaitingNorth = numWaitingNorth;
		}
		
		nwtn++;
		
		if(numOnBridgeSouth > 0){
			/* Block while vehicles are travelling in the opposite direction. */
			while(numOnBridgeSouth > 0){
				printf("Truck %d is blocked due to %d vehicles on the bridge crossing Southbound\n", mystate->id, numOnBridgeSouth);
				fflush(stdout);
				pthread_cond_wait(&northCond, &bridgeLock);
			}
			
		}else if(numOnBridgeSouth == 0){
			/* Block when vehicles are waiting to cross Southbound. */
			if(numWaitingSouth > 0){
				printf("Truck %d is blocked due to %d vehicles waiting to cross Southbound\n", mystate->id, numWaitingSouth);
				fflush(stdout);
				pthread_cond_wait(&northCond, &bridgeLock);
			}
		}
		/* Can now proceed onto the bridge, so update the number of waiting
		   northbound vehicles, the number of vehicles crossing the bridge
			 northbound, and the max number of vehicles on the bridge at once. */
		numWaitingNorth--;
		
		/* When there are no more trucks waiting to cross northbound singal threads
		   waiting on that condition. */
		if(--nwtn == 0){
			pthread_cond_broadcast(&southWaitTruckCond);	
		}
		
		nobtn++;

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

		nwts++;

		if(numOnBridgeNorth > 0){
			/* Block while vehicles are travelling in the opposite direction. */
			while(numOnBridgeNorth > 0){
				printf("Truck %d is blocked due to %d vehicles on the bridge crossing Northbound\n", mystate->id, numOnBridgeNorth);
				fflush(stdout);
				pthread_cond_wait(&southCond, &bridgeLock);
			}
		}else if(numOnBridgeNorth == 0){
			/* Block when vehicles are waiting to cross Northbound. */
			if(numWaitingNorth > 0){
				printf("Truck %d is blocked due to %d vehicles waiting to cross Northbound\n", mystate->id, numWaitingNorth);
				fflush(stdout);
				pthread_cond_wait(&southCond, &bridgeLock);
			}
		}
		/* Can now proceed onto the bridge, so update the number of waiting
		   southbound vehicles, the number of vehicles crossing the bridge
			 southbound, and the max number of vehicles on the bridge at once. */
		numWaitingSouth--;
		
		/* When there are no more trucks waiting to cross Southbound signal
		   the threads that are waiting on that condition. */
		if(--nwts == 0){
			pthread_cond_broadcast(&northWaitTruckCond);	
		}
		
		nobts++;
		
		if(++numOnBridgeSouth > maxOnBridge){
			maxOnBridge = numOnBridgeSouth;
		}
	}
 
	/* Release the bridge lock. */
	pthread_mutex_unlock(&bridgeLock);
	printf("Truck %d is crossing the bridge heading %s\n", mystate->id, DIR2STR(mystate->dir));
	fflush(stdout);
//	printState();
	sleep(1);  /* pass some time on the bridge */

	/* Acquire the bridge lock. */
 	pthread_mutex_lock(&bridgeLock);
	
	printf("Truck %d is exiting the bridge heading %s\n", mystate->id, DIR2STR(mystate->dir));
	fflush(stdout);

	/* If heading north. */
	if(mystate->dir == Northbound){
		
		
		if(numWaitingSouth > 0 && --numOnBridgeNorth == 0){
			pthread_cond_broadcast(&southCond);
		}
	
		/* When there are no more trucks crossing Northbound signal the
		   threads that are waiting on that condition. */
		if(--nobtn == 0){
			pthread_cond_broadcast(&southTruckCond);	
		}

		/* If the last vehicle currently travelling northbound
		   notify the southbound vehicles that they can now
			 cross the bridge. */
		if(numOnBridgeNorth == 0){
			pthread_cond_broadcast(&southCond);
		}

	/* If heading south. */	
	}else{
		
		if(numWaitingNorth > 0 && --numOnBridgeSouth == 0){
			pthread_cond_broadcast(&northCond);
		}

		/* When there are no more trucks crossing Southbound signal the
		   threads that are waiting on that condition. */
		if(--nobts == 0){
			pthread_cond_broadcast(&northTruckCond);	
		}

		/* If the last vehicle currently travelling southbound
		   notify the northbound vehicles that they can now
			 cross the bridge. */
		if(numOnBridgeSouth == 0){
			pthread_cond_broadcast(&northCond);
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

/*******************************************************************************
 * function driveBicycle() - used by a Bicycle thread, simply calls crossBridge() 
 * passing in the current state as a parameter.
 ******************************************************************************/
void *rideBicycle(void *threadarg){
  
	vstate_t *mystate = (vstate_t *)threadarg;
	
	printf("Bicycle %d arrives to the bridge heading %s\n", mystate->id, DIR2STR(mystate->dir));
	fflush(stdout);
	
	/* Acquire bridge lock. */
	pthread_mutex_lock(&bridgeLock);
	
	/* If heading North */
	if(mystate->dir == Northbound){
		
		/* Update the number of waiting northbound vehicles
		   and the maximum number of waiting northbound vehicles. */
		if(++numWaitingNorth > maxWaitingNorth){
			maxWaitingNorth = numWaitingNorth;
		}
		
		nwbn++;
		
		if(numOnBridgeSouth > 0){
			/* Block while trucks are crossing the bridge or waiting to cross the bridge Southbound. */
			while(numOnBridgeSouth > 0 && (nobts > 0 || nwts > 0)){
				
				/* When there are trucks crossing the bridge Southbound. */
				if(nobts > 0){
					printf("Bicycle %d is blocked due to %d trucks on the bridge crossing Southbound\n", mystate->id, nobts);
					fflush(stdout);
					pthread_cond_wait(&northTruckCond, &bridgeLock);
				/* When there are trucks waiting to cross the bridge Southbound. */
				}else if(nwts > 0){
					printf("Bicycle %d is blocked due to %d trucks waiting to cross Southbound\n", mystate->id, nwts);
					fflush(stdout);
					pthread_cond_wait(&northWaitTruckCond, &bridgeLock);
				}
			}
			
		}else if(numOnBridgeSouth == 0){
			/* Block when trucks are waiting to cross the bridge Southbound. */
			if(numWaitingSouth > 0 && nwts > 0){
				printf("Bicycle %d is blocked due to %d trucks waiting to cross Southbound\n", mystate->id, nwts);
				fflush(stdout);
				pthread_cond_wait(&northWaitTruckCond, &bridgeLock);
			}
		}
		/* Can now proceed onto the bridge, so update the number of waiting
		   northbound vehicles, the number of vehicles crossing the bridge
			 northbound, and the max number of vehicles on the bridge at once. */
		numWaitingNorth--;
		
		nwbn--;
		nobbn++;

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

		nwbs++;

		if(numOnBridgeNorth > 0){
			/* Block while trucks are crossing the bridge or waiting to cross the bridge Northbound. */
			while(numOnBridgeNorth > 0 && (nobtn > 0 || nwtn > 0)){
				/* When there are trucks crossing the bridge Northbound. */
				if(nobtn > 0){
					printf("Bicycle %d is blocked due to %d trucks on the bridge crossing Northbound\n", mystate->id, nobtn);
					fflush(stdout);
					pthread_cond_wait(&southTruckCond, &bridgeLock);
				/* When there are trucks waiting to cross the bridge Northbound. */
				}else if(nwtn > 0){
					printf("Bicycle %d is blocked due to %d trucks waiting to cross Northbound\n", mystate->id, nwtn);
					fflush(stdout);
					pthread_cond_wait(&southWaitTruckCond, &bridgeLock);
				}
			}
		}else if(numOnBridgeNorth == 0){
			/* Block when there are trucks waiting to cross the bridge Northbound. */
			if(numWaitingNorth > 0 && nwtn > 0){
				printf("Bicycle %d is blocked due to %d trucks waiting to cross Northbound\n", mystate->id, nwtn);
				fflush(stdout);
				pthread_cond_wait(&southWaitTruckCond, &bridgeLock);
			}
		}
		/* Can now proceed onto the bridge, so update the number of waiting
		   southbound vehicles, the number of vehicles crossing the bridge
			 southbound, and the max number of vehicles on the bridge at once. */
		numWaitingSouth--;
		nwbs--;
		nobbs++;
		
		if(++numOnBridgeSouth > maxOnBridge){
			maxOnBridge = numOnBridgeSouth;
		}
	}
 
	/* Release the bridge lock. */
	pthread_mutex_unlock(&bridgeLock);
	printf("Bicycle %d is crossing the bridge heading %s\n", mystate->id, DIR2STR(mystate->dir));
	fflush(stdout);
//	printState();
	sleep(1);  /* pass some time on the bridge */

	/* Acquire the bridge lock. */
 	pthread_mutex_lock(&bridgeLock);

	printf("Bicycle %d is exiting the bridge heading %s\n", mystate->id, DIR2STR(mystate->dir));
	fflush(stdout);
	
	/* If heading north. */
	if(mystate->dir == Northbound){
		
		
		if(numWaitingSouth > 0 && --numOnBridgeNorth == 0){
			pthread_cond_broadcast(&southCond);
		}
	
		nobbn--;

		/* If the last vehicle currently travelling northbound
		   notify the southbound vehicles that they can now
			 cross the bridge. */
		if(numOnBridgeNorth == 0){
			pthread_cond_broadcast(&southCond);
		}

	/* If heading south. */	
	}else{
		
		if(numWaitingNorth > 0 && --numOnBridgeSouth == 0){
			pthread_cond_broadcast(&northCond);
		}

		nobbs--;

		/* If the last vehicle currently travelling southbound
		   notify the northbound vehicles that they can now
			 cross the bridge. */
		if(numOnBridgeSouth == 0){
			pthread_cond_broadcast(&northCond);
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


void printState(){
	pthread_mutex_lock(&textLock);
	printf("Cars on bridge North: %d, Cars waiting North: %d\nCars on bridge South: %d, Cars waiting South: %d\nBikes on bridge North: %d, Bikes waiting North: %d\nBikes on bridge South: %d, Bikes waiting South: %d\nTrucks on bridge North: %d, Trucks waiting North: %d\nTrucks on bridge South: %d, Trucks waiting South: %d\n\n\n", nobcn, nwcn, nobcs, nwcs, nobbn, nwbn, nobbs, nwbs, nobtn, nwtn, nobts, nwts);
	fflush(stdout);
	pthread_mutex_unlock(&textLock);

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

	/* Initialize thread counter lock. */
	pthread_mutex_init(&textLock, NULL);

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
