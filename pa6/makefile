COMPILER = gcc
CCFLAGS  = -pthread -Wall

all: multithread

debug:
	make DEBUG=TRUE

multithread: pcThread.o queue.o tokenizer.o 
	$(COMPILER) $(CCFLAGS) -o multithread pcThread.o queue.o tokenizer.o 
	
pcThread.o: pcThread.c pcThread.h
	$(COMPILER) $(CCFLAGS) -c pcThread.c

queue.o: queue.c queue.h
	$(COMPILER) $(CCFLAGS) -c queue.c

tokenizer.o: tokenizer.c tokenizer.h
	$(COMPILER) $(CCFLAGS) -c tokenizer.c
	
ifeq ($(DEBUG), TRUE)
CCFLAGS += -g
endif

clean:
	rm -f multithread
	rm -f *.o
