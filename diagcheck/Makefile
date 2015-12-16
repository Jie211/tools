##
##macro
##
SOURCES = function.c\
					diagonallyCheaker.c

#bin file
BIN = DiagCheaker

#object file
OBJS = $(SOURCES)

CC = gcc

#cc flags
CCFLAGS = -lm -O2 -Wall -std=c99 -fopenmp 

#debug
DEBUGFLAGS = -g -DEBUG

#time
TIMEFLAGS = -DTIME

##
## make rule
## 

#compile
all:$(BIN)

#clean
clear:
	rm $(BIN)

#link
$(BIN): $(OBJS)
	$(CC)  $(OBJS) -o $(BIN) $(CCFLAGS)

debug: CCFLAGS += $(DEBUGFLAGS)
debug: all

time: CCFLAGS += $(TIMEFLAGS)
time: all

# ##
# ##suffixes
# ##
#
# %.o : %.cu
# 	$(CC) $(CCFLAGS)  -o $@ $<
