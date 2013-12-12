##############################################################################
##
##
##  GNU Makefile for 
##  Lab assignment 3 (CSC 369, Fall 2003)
##
##
##############################################################################

NAME=unknown
VERSION=1
GPROF=

ASSTDIR=/h/u2/csc369h/fall/pub/A3

# Uncomment only one of the following three:
DEFINES=-DLINUX
#DEFINES=-DALPHA
#DEFINES=

CC=gcc

#
# uncomment one of the two lines below
#
CFLAGS=-Wall $(GPROF) -lpthread -g $(DEFINES)
#CFLAGS=-Wall -O2 $(DEFINES)
#
#
CPP=gcc -E

INCDIR=
LIBDIR=

LIBS=

OBJS=memlib.o csapp.o malloc.o

##############################################################################

all: all-target

ifeq (Makefile.depend, $(wildcard Makefile.depend))
include Makefile.depend
all-target: malloc
else
all-target: depend
	@echo Please run make once again!
endif

##############################################################################

malloc: $(OBJS)
	$(CC) $(CFLAGS) $(INCDIR) $(LIBDIR) $(OBJS) $(LIBS) -o malloc

ftime.o: 
	$(CC) -O2 $(INCDIR) $(LIBDIR) -c -o $*.o $<

dump.o:
	$(CC) $(CFLAGS) $(INCDIR) $(LIBDIR) -c -o $*.o $<

memlib.o:
	$(CC) $(CFLAGS) $(INCDIR) $(LIBDIR) -c -o $*.o $<

malloc.o:
	$(CC) $(CFLAGS) $(INCDIR) $(LIBDIR) -c -o $*.o $<
csapp.o:
	$(CC) $(CFLAGS) $(INCDIR) $(LIBDIR) -c -o $*.o $<
driver.o:
	$(CC) $(CFLAGS) $(INCDIR) $(LIBDIR) -c -o $*.o $<

depend dep:
	@echo Generating dependencies...
	$(CPP) -M $(CPPFLAGS) $(INCDIR) *.c >Makefile.depend

edclean:
	-rm *.bak *~

clean: edclean
	-rm -f $(OBJS) core
	-rm -f Makefile.depend

nuke: clean
	-rm -f malloc

##############################################################################




