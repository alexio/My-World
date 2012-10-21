COMPILER = gcc
CCFLAGS  = -Wall -g

all: index

debug:
	make DEBUG=TRUE

index: main.o indexer.o hash.o tokenizer.o
	$(COMPILER) $(CCFLAGS) -o index main.o indexer.o hash.o tokenizer.o

main.o: main.c
	$(COMPILER) $(CCFLAGS) -c main.c

indexer.o: indexer.c indexer.h
	$(COMPILER) $(CCFLAGS) -c indexer.c

hash.o: hash.c hash.h
	$(COMPILER) $(CCFLAGS) -c hash.c

tokenizer.o: tokenizer.c tokenizer.h
	$(COMPILER) $(CCFLAGS) -c tokenizer.c

ifeq ($(DEBUG), TRUE)
CCFLAGS += -g
endif

clean:
	rm -f index
	rm -f *.o
