##
##macro
##
SOURCES = main.c\
					function.c\

#bin file
BIN = Checker

#object file
OBJS = $(SOURCES)

CC = gcc48

#cc flags
CCFLAGS = -O3 -lm -Wall -std=c99 -fopenmp 

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
