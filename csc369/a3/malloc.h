#ifndef __MALLOC_H_
#define __MALLOC_H_

/* $Id$ */

/*
 *
 *  CSC 369 - Assignment 3
 *
 */

#include <stdio.h>

extern int mm_init (void);
extern void *mm_malloc (size_t size);
extern void mm_free (void *ptr);

/* Team information */
typedef struct {
    char *name;
    char *fullname, *emailaddr;
} name_t;

extern name_t myname;


#endif /* __MALLOC_H_ */

