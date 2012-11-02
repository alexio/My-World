COMPILER = gcc
CCFLAGS  = -Wall -g

all: index

debug:
	make DEBUG=TRUE

index: main.o indexer.o tree.o tokenizer.o
	$(COMPILER) $(CCFLAGS) -o index main.o indexer.o tree.o tokenizer.o

main.o: main.c
	$(COMPILER) $(CCFLAGS) -c main.c

indexer.o: indexer.c indexer.h
	$(COMPILER) $(CCFLAGS) -c indexer.c

tree.o: tree.c tree.h
	$(COMPILER) $(CCFLAGS) -c tree.c

tokenizer.o: tokenizer.c tokenizer.h
	$(COMPILER) $(CCFLAGS) -c tokenizer.c

ifeq ($(DEBUG), TRUE)
CCFLAGS += -g
endif

clean:
	rm -f index
	rm -f *.o
