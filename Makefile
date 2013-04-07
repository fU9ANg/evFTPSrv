
#
# makefile
#
# by fU9ANg
# bb.newlife@gmail.com
#

# Which c++ compiler
CC          = ccache gcc

# Which c compiler
CPP         = ccache g++

# Binary
BIN         = evFTPSrv

# Objects
OBJS        = d2FTPTask.o       \
              d2SendTask.o      \
              d2EvLoopTask.o    \
              d2ThreadPool.o    \
              d2MemBlock.o      \
              d2SingleFactory.o \
              Main.o

# Options for development
CFLAGS      = -Wall -Werror -g

# Options for release
#CFLAGS  = -O3 -Wall -Werror
# Local libraries
LIBS        = -lev -lpthread

# Where are include files kept
INCS        = -I.
LIB_PATH    = -L.

all:$(BIN)

$(BIN):$(OBJS)
	$(CPP) $(CFLAGS) $(INCS) -o $@ $(OBJS) $(LIB_PATH) $(LIBS)

%.o:%.cpp
	$(CPP) $(CFLAGS) $(INCS) -o $@ -c $<

%.o:%.c
	$(CC)  $(CFLAGS) $(INCS) -o $@ -c $<

.PHONY: clean
clean:
	-rm -rf $(OBJS) evFTPSrv *~
