/*
 * etime.c - timer package that uses the Unix interval timer
 *           to measure elapsed time in seconds.
 */
#include <stdio.h>
#include <sys/time.h>
#include "etime.h"

/* static variable that holds the initial value of the interval timer */
struct itimerval first_u; /* user time */
struct itimerval first_r; /* real time */
struct itimerval first_p; /* prof time */

/* 
 * elapsed user time routines 
 */

/* init the timer */
void init_etime(void)
{
    first_u.it_interval.tv_sec = 0;
    first_u.it_interval.tv_usec = 0;
    first_u.it_value.tv_sec = MAX_ETIME;
    first_u.it_value.tv_usec = 0;
    setitimer(ITIMER_VIRTUAL, &first_u, NULL);
}

/* return elapsed seconds since call to init_etime */
double get_etime(void) {
    struct itimerval curr;

    getitimer(ITIMER_VIRTUAL, &curr);
    return (double) ((first_u.it_value.tv_sec - curr.it_value.tv_sec)*1000 +
		     (first_u.it_value.tv_usec - curr.it_value.tv_usec)*1e-3);
}

/* 
 * elapsed real time routines 
 */

/* init the timer */
void init_etime_real(void)
{
    first_r.it_interval.tv_sec = 0;
    first_r.it_interval.tv_usec = 0;
    first_r.it_value.tv_sec = MAX_ETIME;
    first_r.it_value.tv_usec = 0;
    setitimer(ITIMER_REAL, &first_r, NULL);
}

/* return elapsed seconds since call to init_etime_real */
double get_etime_real(void) {
    struct itimerval curr;

    getitimer(ITIMER_REAL, &curr);
    return (double) ((first_r.it_value.tv_sec - curr.it_value.tv_sec)*1000 +
		     (first_r.it_value.tv_usec - curr.it_value.tv_usec)*1e-3);
}

/* 
 * elapsed prof time routines 
 */

/* init the timer */
void init_etime_prof(void)
{
    first_p.it_interval.tv_sec = 0;
    first_p.it_interval.tv_usec = 0;
    first_p.it_value.tv_sec = MAX_ETIME;
    first_p.it_value.tv_usec = 0;
    setitimer(ITIMER_PROF, &first_p, NULL);
}

/* return elapsed seconds since call to init_etime_prof */
double get_etime_prof(void) {
    struct itimerval curr;

    getitimer(ITIMER_PROF, &curr);
    return (double) ((first_p.it_value.tv_sec - curr.it_value.tv_sec)*1000 +
		     (first_p.it_value.tv_usec - curr.it_value.tv_usec)*1e-3);
}

