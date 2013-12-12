/******************************************
 * Assignment 3
 * CSC 354
 * Sergei Iakhnin
 * 981 843 420
 ******************************************/

#include "csim.h"

#define MAX_CUST 3

EVENT done;		/*pointer for completion event */

EVENT maxCust; /*pointer for event signaling maximum number of customers in system*/


FACILITY f1;
FACILITY f2;

/* customer interarrival rate(lambda parameter) */
double iatm;

/* customer service rate(mu parameter) */
double svtm;

/* current number of customers in system */
int num_cust;

/* Customers to simulate per run. */
int max_cust;

/* Number of warm-up customers to discard on each run. */
int num_transients;

/* Counter of customers generated on current run. */
int custCtr = 0;

/* Main simulation process. */
void sim(int whichSim, double *mtis, double *mtiq, double *mu, double* mt);

/* Customer for strategy S*/
void cust();

/* Customer for strategy R*/
void cust2();

/* Initialze data structures for simulation. */
void init();

/* Program entry-point. */
int main(int argc, char *argv[]){
	
	/* Number of replications. */
	int numRep = 0, numRep2 = 0;
	
	/* Flag for switching between strategies. */
	int whichSim = 0;
	int i = 0;
	
	/* Estimated means for statistics of interest. */
	double e1 = 0, e2 = 0, e3 = 0, e4 = 0;
	
	/* Estimated variances for the means of statistics of interest. */
	double v1 = 0, v2 = 0, v3 = 0, v4 = 0;
	
	/* Mean time in system, mean time in queue, mean utilization, mean throughput. */
	double mtis, mtiq, mu, mt;
	
	/* Obtain customer interarrival time.*/
	printf("Enter mean inter-arrival time\n");
	scanf("%lf", &iatm);
	
	/* Obtain customer service time. */
	printf("Enter mean service time\n");
	scanf("%lf", &svtm);
	
	/* Obtain number of customers to generate per run. */
	printf("Enter number of customers per day\n");
	scanf("%d", &max_cust);
	
	/* Obtain number of customers to discard as part of system warm-up on each run. */
	printf("Enter number of transients\n");
	scanf("%d", &num_transients);
	
	/* Obtain number of runs to perform.*/
	printf("Enter number of replications to perform\n");
	scanf("%d", &numRep);
	
	/* Allocate memory for holding statistics of interest for each replication. */
	double *data = (double*)malloc(4 * sizeof(double) * numRep);
	
	
	numRep2 = numRep * 2;
	
	
	double *dataStart;
	dataStart = data;
	
	/* Perform numRep * 2 replications of the simulation (numRep for each strategy) */
	while(numRep2-- > 0){
		
		/* Run simulation. */
		sim(whichSim, &mtis, &mtiq, &mu, &mt);
		
		/* If simulated strategy S*/
		if(whichSim == 0){
			
			/* Set strategy R as next up for simulation */
			whichSim = 1;
			
			/* Tally up statistics of interest. */
			*data = mtis;
			*(data + 1) = mtiq;
			*(data + 2) = mu;
			*(data + 3) = mt; 
		/* If simulated strategy R */
		}else{
			/* Set strategy S as next up for simulation */
			whichSim = 0;
			
			/* Tally up statistics of interest. */
			*data -= mtis;
			*(data + 1) -= mtiq;
			*(data + 2) -= mu;
			*(data + 3) -= mt; 
				
			/* Advance pointer into data array */
			data += 4;
		}
	
		/* Clear simulation data structures for next run. */
		rerun();
	}
	
	/* Calculate means for statistics of interes. */
	for(i = 0; i < numRep; i++){
		e1 += *(dataStart + 4 * i);
		e2 += *(dataStart + 4 * i + 1);
		e3 += *(dataStart + 4 * i + 2);
		e4 += *(dataStart + 4 * i + 3);
	}
	
	e1 /= numRep;
	e2 /= numRep;
	e3 /= numRep;
	e4 /= numRep;
	
	/* Calculate variances of the means of statistics of interes. */
	for(i = 0; i < numRep; i++){
		v1 += (*(dataStart + 4 * i) - e1) * (*(dataStart + 4 * i) - e1);
		v2 += (*(dataStart + 4 * i + 1) - e2) * (*(dataStart + 4 * i + 1) - e2);
		v3 += (*(dataStart + 4 * i + 2) - e3) * (*(dataStart + 4 * i + 2) - e3);
		v4 += (*(dataStart + 4 * i + 3) - e4) * (*(dataStart + 4 * i + 3) - e4);
	}
	
	int val =  numRep * (numRep - 1);
	
	v1 /= val;
	v2 /= val;
	v3 /= val;
	v4 /= val;
	
	/* Print out the computed means as well as standard deviations of the means
	for the statistics of interest. */
	printf("%lf %lf\n", e1, sqrt((double)v1));
	
	printf("%lf %lf\n", e2, sqrt((double)v2));
	
	printf("%lf %lf\n", e3, sqrt((double)v3));
	
	printf("%lf %lf\n", e4, sqrt((double)v4));
	
	/* Complete simulation. */
	conclude_csim();

	return 0;
}

/* Main simulation process. */
void sim(int whichSim, double *mtis, double *mtiq, double *mu, double* mt){
	custCtr = 0;
	
	create("sim");
	
	
	/* Initialize simulation. */
	init();
	
	/* For the duration of the simulation generate customers with 
	   exponential inter-arrival times with mean iatm. */
	while(custCtr < max_cust){
		/* If the number of customers in system does not exceed the maximum number allowed. */
		if(num_cust < MAX_CUST){
			
			hold(exponential(iatm));
			
			/* Reset the stats counters when the system has been warmed up */
			if(custCtr == num_transients){
				reset();
			}
			
			custCtr++;
			
			/* Generate customers according to which strategy is currently being simulated. */
			if(whichSim == 0){
				cust();
			}else{
				cust2();
			}
		
		/* Otherwise wait for a signal that the number of customers in the system permits creation of more. */
		}else{
			wait(maxCust);
		}
		
	}
	
	/* Wait until all customers have completed simulation.*/
	wait(done);
	
	/* Mean time of customer in system. */
	(*mtis) = (resp(f1) + resp(f2)) / 2;
	
	/* Mean time of customer in queue. */
	(*mtiq) = (*mtis) - (serv(f1) + serv(f2)) / 2;
	
	/* Mean utilization. */
	(*mu) = (util(f1) + util(f2)) / 2;
	
	/* Mean throughput. */
	(*mt) = (tput(f1) + tput(f2)) / 2;
	
}

/* Initialize simulation data structures. */
void init(){
	
	
	num_cust = 0;

	/* Initialize two facilities to represent servers. */
	f1 = facility("server 1");
	f2 = facility("server 2");
	
	/* Initialize an event to signal simulation completion */
	done = event("done");

	/* Initialize an event to signal that the maximum number of customers in the system has been reached. */
	maxCust = event("maxCust");
	
}

/* Customer for strategy S. */
void cust(){
	
	/* Make sure we are allowed to simulate another customer. */
	if(custCtr <= max_cust){
	
		/* Increase counter of customers in the system. */
		num_cust++;

		create("cust");

		/* Service customer at cashier. */
		use(f1, exponential(svtm));
		
		/* Service customer at dispenser. */
		use(f2, exponential(svtm));


		/* Decrease counter of customers in the system. */
		num_cust--;

		/* Signal for creation of more customers. */
		if(num_cust < MAX_CUST){
			set(maxCust);
		}


		/* Check if time to terminate. */
		if(custCtr >= max_cust){
			set(done);
		}
	}
}

/* Customer for strategy S. */
void cust2(){
	/* Make sure we are allowed to simulate another customer. */
	if(custCtr <= max_cust){
	
		/* Increase counter of customers in the system. */
		num_cust++;
	

		create("cust");
		
		/* Determine what server customer goes to. */
		if(uniform(0,1) < 0.5){

			reserve(f1);
			{
				/* Take customer order. */
				hold(exponential(svtm));
				
				/* Prepare customer order. */
				hold(exponential(svtm));
			}
			release(f1);

		}else{
			reserve(f2);
			{
				/* Take customer order. */
				hold(exponential(svtm));

				/* Perpare customer order. */
				hold(exponential(svtm));
			}
			release(f2);

		}
		

		/* Decrease counter of customers in the system. */
		num_cust--;
	
		/* Can signal for arrival of more customers. */
		if(num_cust < MAX_CUST){
			set(maxCust);
		}
	
		/* Check if time to terminate. */
		if(custCtr >= max_cust){
			set(done);
		}
		
	}
}
