
/* simple example of how to use the etime package */

#include <stdio.h>
#include "etime.h"

main(int argc, char **argv) {
    int i;
    int a=1;
    double start_time, end_time, msecs;

    /* call init_etime to initialize the timer value */
    init_etime_prof();

    start_time = get_etime_prof();
    for (i=0; i<100000; i++)
      a += i/a;
    end_time = get_etime_prof();
    msecs = end_time - start_time;

    printf("The loop computed %d and took %f msecs\n", a, msecs);
}
