
#
# makefile
#
# by fU9ANg
# bb.newlife@gmail.com
#

# Which c++ compiler
CC          = ccache cc

# Which c compiler
CPP         = ccache g++

# Binary
BIN         = evFTPSrv evFTPClient

# Objects
OBJS        = d2FTPTask.o       	\
              d2SendTask.o      	\
              d2ThreadPool.o    	\
              d2MemBlock.o      	\
              d2SingleFactory.o 	\
              d2EvLoopTask.o    	\
              d2EpollLoopTask.o 	\
              d2SelectLoopTask.o	\
              Main.o

CLIENTOBJS  = client.o

# Options for development
CFLAGS      = -Wall -Werror -g -D_ONLY_TEST
CXXFLAGS    = -Wall -Werror -g -D_ONLY_TEST

# Options for release
#CFLAGS  = -O3 -Wall -Werror
# Local libraries
LIBS        = -lev -lpthread

# Where are include files kept
INCS        = -I.
LIB_PATH    = -L.

all:$(BIN)

evFTPSrv:$(OBJS)
	$(CPP) $(CXXFLAGS) $(INCS) -o $@ $(OBJS) $(LIB_PATH) $(LIBS)

evFTPClient:$(CLIENTOBJS)
	$(CC)  $(CFLAGS)   $(INCS) -o $@ $(CLIENTOBJS) $(LIB_PATH) $(LIBS)

%.o:%.cpp
	$(CPP) $(CFLAGS) $(INCS) -o $@ -c $<

%.o:%.c
	$(CC)  $(CFLAGS) $(INCS) -o $@ -c $<

.PHONY: clean
clean:
	-rm -rf $(OBJS) $(CLIENTOBJS) $(BIN) *~
