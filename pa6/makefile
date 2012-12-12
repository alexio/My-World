COMPILER = gcc
CCFLAGS  = -pthread -lm -Wall

all: multithread

debug:
	make DEBUG=TRUE

multithread: main.o queue.o tokenizer.o
	$(COMPILER) $(CCFLAGS) -o multithread main.o queue.o tokenizer.o
	
main.o: main.c
	$(COMPILER) $(CCFLAGS) -c main.c

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
