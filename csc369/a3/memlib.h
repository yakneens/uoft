#ifndef __MEMLIB_H_
#define __MEMLIB_H_

/* $Id$ */

/*
 *  CSC 369 - Assignment 3
 *
 */

#include <unistd.h>

#ifndef ptrdiff_t
# define ptrdiff_t       int
#endif


#define DSEG_MAX 20*1024*1024  /* 20 Mb */

extern char *dseg_lo, *dseg_hi;
extern long dseg_size;

extern int mem_init (void);
extern int mem_reinit (long size);
extern void *mem_sbrk (ptrdiff_t increment);
extern int mem_pagesize (void);
extern long mem_usage (void);

#endif /* __MEMLIB_H_ */

