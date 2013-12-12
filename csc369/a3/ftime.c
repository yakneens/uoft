/* Utility for timing function evaluations */

#include <stdio.h>
#include <sys/time.h>
#include <limits.h>
#include "ftime.h"

/* Detect whether running on x86 */
#ifdef __i386
#define IS_x86 1
#else
#define IS_x86 0
#endif

//#define IS_x86 1

/* Do I want clock speed info? */
#define VERBOSE !IS_x86

#define MAX_ETIME 86400   

/* Computed clock speed */
static double mhz = 0.0;

/* static variable that holds the initial value of the interval timer */
static struct itimerval first_u; /* user time */
static struct itimerval first_r; /* real time */

/* 
 * elapsed user time routines 
 */

/* init the timer */
static void init_etime(void)
{
    first_u.it_interval.tv_sec = 0;
    first_u.it_interval.tv_usec = 0;
    first_u.it_value.tv_sec = MAX_ETIME;
    first_u.it_value.tv_usec = 0;
    setitimer(ITIMER_VIRTUAL, &first_u, NULL);
    first_r.it_interval.tv_sec = 0;
    first_r.it_interval.tv_usec = 0;
    first_r.it_value.tv_sec = MAX_ETIME;
    first_r.it_value.tv_usec = 0;
    setitimer(ITIMER_REAL, &first_r, NULL);
}

/* return elapsed virtual seconds since call to init_etime */
static double get_etime(void) {
    struct itimerval curr;

    getitimer(ITIMER_VIRTUAL, &curr);
    return (double) ((first_u.it_value.tv_sec - curr.it_value.tv_sec) +
		     (first_u.it_value.tv_usec - curr.it_value.tv_usec)*1e-6);
}

/* return elapsed real seconds since call to init_etime */
static double get_rtime(void) {
    struct itimerval curr;

    getitimer(ITIMER_REAL, &curr);
    return (double) ((first_r.it_value.tv_sec - curr.it_value.tv_sec) +
		     (first_r.it_value.tv_usec - curr.it_value.tv_usec)*1e-6);
}

/* Need to compute value of timer delay */
static double delta_t = 0.0;

static double ftime_default(test_funct P, double E)
{
  int cnt = 1;
  double tmin;
  double tmeas = 0.0;
  /* Make sure timer interval has been computed */
  if (delta_t == 0.0) {
    double start;
    init_etime();
    start = get_etime();
    while ((delta_t = get_etime() - start) <= 1e-6)
      ;
#if  VERBOSE
    printf("Delta = %0.3f\n", delta_t);
#endif
  }
  tmin = delta_t / E + delta_t;
  while (tmeas < tmin) {
    int c = cnt;
    double start = get_etime();
    while (c-- > 0) {
      P();
    }
    tmeas = get_etime() - start;
    if (tmeas < tmin)
      cnt += cnt;
  }
  return tmeas / cnt;
}

/* Routine to access x86 cycle counter */
unsigned dummy = 31415;


/* Get timer value as int */
/* Must be compiled -O */
unsigned counter()
{
  unsigned result = dummy;
  asm("rdtsc"); /* Puts cycle count in %eax */
  return result;
}



/* Minimum number of cycles before satisfied with accuracy of cycle counter */
#define CMIN 50000

static volatile unsigned sink;

/* Do single timing on x86 */
static double ftime_x86_single(test_funct P, double E) {
  int cnt = 1;
  int cmeas = 0;
  double start, real_start;
  int delta_count = 0;
  int real_delta_count = 0;
  double delta_fraction;
  double cycles;
  int interrupted = 0;
  if (mhz == 0)
    mhz = freq();
  do  {
    int c = cnt;
    int start_cycle;
    init_etime();     /* Want to see if fit within timer interval */
    /* Call everything once to eliminate cache startup effects */
    sink = counter();
    P();
    start = get_etime();
    real_start = get_rtime();
    start_cycle = counter();
    while (c-- > 0)
      P();
    cmeas = counter() - start_cycle;
    delta_count = (get_etime() - start)/delta_t;
    real_delta_count = (get_rtime() - real_start)/delta_t;
    cycles = (double) cmeas / cnt;
    delta_fraction = cmeas * 1e-6 / mhz / delta_t;
/*
    if (delta_fraction >= 1.0)
      printf("Delta Fraction = %.2f\n", delta_fraction);
*/
    interrupted = real_delta_count > delta_count;
/*
    if (interrupted && delta_count >= 10) {
      printf("%d real delta's > %d virtual delta's, but giving up ...\n", real_delta_count,
	     delta_count);
    }
*/
  if(delta_fraction < 0.5)
    cnt += cnt;
  } while (cmeas < CMIN ||
	   (interrupted && delta_count < 10) ||
	   (delta_count > 0 && delta_fraction <= 0.75));
/*
  if (interrupted) {
    printf("Ran %d times getting avg cycles = %.2f (%.2fns), %d delta T's\n",
	   cnt/2, cycles, cycles * 1e3 / mhz, delta_count);
    if (delta_fraction > 0.75) {
      printf("Could not get precise timing.  Ran %.2f delta\n", delta_fraction);
    }
  }
*/
  return cycles * 1e-6 / mhz;
}

/* Run multiple trials on x86 */
/* Try to get two fastest runs to be within TRIAL_TOL of each other */
#define MIN_TRIALS 2
#define MAX_TRIALS 10

#define TRIAL_TOL 0.001
static double ftime_x86(test_funct P, double E) {
  double vals[MAX_TRIALS];
  int trials;
  int done = 0;
  for (trials = 0; trials < MAX_TRIALS && !done; trials++) {
    double secs = ftime_x86_single(P, E);
    int i;
    /* Insertion sort */
    for (i = trials; i > 0 && secs < vals[i-1]; i--) 
      vals[i] = vals[i-1];
    vals[i] = secs;
    done = trials >= 2 && vals[1] <= vals[0]*(1.0+E);
  }
  /* Debugging */
/*
  printf("%d Trials.  Values (usecs):", trials);
  for (t = 0; t < trials; t++)
    printf("\t%.2f", vals[t]*1e6);
  printf("\n");
  if (!done)
    printf("Inconsistent Measurement\n");
*/
  return vals[0];
}

double ftime(test_funct P, double E) {
#if IS_x86
  return ftime_x86(P, E);
#else
  return ftime_default(P, E);
#endif
}

/* Some memory locations to prevent dead code elimination */
int ax = 0;
int bx = 0;
int cx = 0;


/* Compute peformance of CPU doing repeated additions */
static void add_test(void)
{
  int a = ax;
  int b = bx;
  int c = cx;
  a += b; a += c; a += b; a += c; a += b; a += c; a += b; a += c; a += b; a += c;
  a += b; a += c; a += b; a += c; a += b; a += c; a += b; a += c; a += b; a += c;
  a += b; a += c; a += b; a += c; a += b; a += c; a += b; a += c; a += b; a += c;
  a += b; a += c; a += b; a += c; a += b; a += c; a += b; a += c; a += b; a += c;
  a += b; a += c; a += b; a += c; a += b; a += c; a += b; a += c; a += b; a += c;
  a += b; a += c; a += b; a += c; a += b; a += c; a += b; a += c; a += b; a += c;
  a += b; a += c; a += b; a += c; a += b; a += c; a += b; a += c; a += b; a += c;
  a += b; a += c; a += b; a += c; a += b; a += c; a += b; a += c; a += b; a += c;
  a += b; a += c; a += b; a += c; a += b; a += c; a += b; a += c; a += b; a += c;
  a += b; a += c; a += b; a += c; a += b; a += c; a += b; a += c; a += b; a += c;

  a += b; a += c; a += b; a += c; a += b; a += c; a += b; a += c; a += b; a += c;
  a += b; a += c; a += b; a += c; a += b; a += c; a += b; a += c; a += b; a += c;
  a += b; a += c; a += b; a += c; a += b; a += c; a += b; a += c; a += b; a += c;
  a += b; a += c; a += b; a += c; a += b; a += c; a += b; a += c; a += b; a += c;
  a += b; a += c; a += b; a += c; a += b; a += c; a += b; a += c; a += b; a += c;
  a += b; a += c; a += b; a += c; a += b; a += c; a += b; a += c; a += b; a += c;
  a += b; a += c; a += b; a += c; a += b; a += c; a += b; a += c; a += b; a += c;
  a += b; a += c; a += b; a += c; a += b; a += c; a += b; a += c; a += b; a += c;
  a += b; a += c; a += b; a += c; a += b; a += c; a += b; a += c; a += b; a += c;
  a += b; a += c; a += b; a += c; a += b; a += c; a += b; a += c; a += b; a += c;

  ax = a + b + c;
}

static void add_dummy(void)
{
  int a = ax;
  int b = bx;
  int c = cx;
  ax = a + b + c;
}

/* Compute clock frequency in MHz */
double freq() {
  /* Compute clock frequency */
  double secs;
  double atime, dtime;
  if (mhz > 0.0)
    return mhz;
  atime = ftime_default(add_test, 0.01);
  dtime = ftime_default(add_dummy, 0.01);
  
  secs = atime - dtime; 
  mhz = (200.0/secs)/1000000.0;
#if VERBOSE
  printf("The clock frequency is approximately %0.1f Megahertz\n", mhz);
#endif
  return mhz;
}
