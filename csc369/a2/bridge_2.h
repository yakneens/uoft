#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

typedef enum direction {
  Northbound,
  Southbound
} Direction_t;

typedef enum vehicle {
  Car,
  Truck,
  Bicycle
} Vehicle_t;

/* vehicle state structure to pass arguments to threads */
typedef struct vstate {
  int id;
  Vehicle_t type;
  Direction_t dir;
} vstate_t;

#define MAX_THREADS 10

/* Macro to print debug output.  Compile with -DDEBUG to turn on.
 * use as DGB(fprintf(stderr,"Some debugging string\n"));
 */
#ifdef DEBUG
#define DBG(_x) \
  ( fprintf(stderr,"%s %d: ", __FILE__, __LINE__), (void)(_x))
#else
#define DBG(_x) 
#endif

/* returns a direction _dir as a string */
#define DIR2STR(_dir) ((_dir) == Northbound ? "Northbound" : "Southbound")

/* returns the opposite direction of _dir */
#define OPP_DIR(_dir) ((_dir) == Northbound ? Southbound : Northbound)

void *driveCar(void *threadarg);
void *driveTruck(void *threadarg);
void *rideBicycle(void *threadarg);
void crossBridge(vstate_t *);

