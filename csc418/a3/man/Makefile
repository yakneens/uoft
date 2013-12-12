#### uncomment one of the following

#include Makefile.pcoffice
include Makefile.cdf
#include Makefile.scar

ARFLAGS       = ruv

DEST          = .

EXTHDRS       = /usr/include/stdio.h

CPPFLAGS      = -I$(GLUIDIR) -I$(XINCDIR)

INCDIR        =  $(GLUIDIR)/include

LDFLAGS       = -L$(GLUIDIR)/lib

LIBS          = $(GL_LIBS) -lm $(XLIBS) -ldl

LINKER        = g++

MAKEFILE      = Makefile

OBJS = $(CPPSRCS:.cpp=.o) $(CSRCS:.c=.o)

PROGRAM       = dude

RANLIB        = ranlib

CPPSRCS = \
        dude.cpp cardinal.cpp ui.cpp

all:            $(PROGRAM)

#$(PROGRAM):     $(OBJS)
#               @echo -n "Loading $(PROGRAM) ... "
#               $(LINKER) $(LDFLAGS) $(OBJS) $(LIBS) -o $(PROGRAM)
#               @echo "done"

%.o : %.cpp
	$(CCC) $(CCCFLAGS) $(CPPFLAGS) $*.cpp

%.o : %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $*.c

$(PROGRAM):     $(OBJS)
	@echo -n "Loading $(PROGRAM) ... "
	$(LINKER) $(LDFLAGS) $(OBJS) $(LIBS) -o $(PROGRAM)
	@echo "done"


clean:;         @rm -f $(OBJS) *~ $(PROGRAM)

tags:    $(SRCS); @etags $(SRCS)

tar:
	cd ..; tar cvf winsim.tar winsim; gzip winsim.tar; \
 	mcopy winsim.tar.gz a:winsim.tgz; echo Done








