##
##macro
##
SOURCES = main.cu\
					function.cu\
#bin file
BIN = Sovler 
#object file
OBJS = $(SOURCES)
#cuda install path
CUDA_PATH = /usr/local/cuda-5.5
#nvcc
NVCC = $(CUDA_PATH)/bin/nvcc
#includes path
INCLUDES = -I $(CUDA_PATH)/samples/common/inc
#nvcc flags
NVCCFLAGS = -O3 -arch=sm_35 -use_fast_math
#debug
DEBUGFLAGS = -g -G -DEBUG
#time
TIMEFLAGS = -DTIME
#history
HISFLAGS = -DHIS
##
## make rule
## 

#compile
all:$(BIN)

#run
run:$(BIN)
	./$(BIN)
#clean
clear:
	$(RM) $(BIN)
#link
$(BIN): $(OBJS)
	$(NVCC)  $(OBJS) -o $(BIN) $(INCLUDES) $(NVCCFLAGS)
debug: NVCCFLAGS += $(DEBUGFLAGS)
debug: all
time: NVCCFLAGS += $(TIMEFLAGS)
time: all
his: NVCCFLAGS += $(HISFLAGS)
his: all
# ##
# ##suffixes
# ##
#
# %.o : %.cu
# 	$(NVCC) $(NVCCFLAGS) -c $(INCLUDES) -o $@ $<
