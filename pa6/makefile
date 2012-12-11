COMPILER = gcc
CCFLAGS  = -pthread -lm -ansi -pedantic -Wall

all: multithread

debug:
	make DEBUG=TRUE

multithread: multithread.o
	$(COMPILER) $(CCFLAGS) -o multithread multithread.o
	
multithread.o: multithread.c
	$(COMPILER) $(CCFLAGS) -c multithread.c
	
ifeq ($(DEBUG), TRUE)
CCFLAGS += -g
endif

clean:
	rm -f multithread
	rm -f *.o
