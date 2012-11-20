COMPILER = gcc
CCFLAGS  = -Wall -g -ansi 
all: search

debug:
	make DEBUG=TRUE

search: search.o searcher.o cache.o hash.o tokenizer.o
	$(COMPILER) $(CCFLAGS) -o search search.o searcher.o cache.o hash.o tokenizer.o
search.o: search.c
	$(COMPILER) $(CCFLAGS) -c search.c
searcher.o: searcher.c searcher.h
	$(COMPILER) $(CCFLAGS) -c searcher.c
cache.o: cache.c cache.h
	$(COMPILER) $(CCFLAGS) -c cache.c
hash.o: hash.c hash.h
	$(COMPILER) $(CCFLAGS) -c hash.c
tokenizer.o: tokenizer.c tokenizer.h
	$(COMPILER) $(CCFLAGS) -c tokenizer.c

ifeq ($(DEBUG), TRUE)
 CCFLAGS += -g
endif

clean:
	rm -f search
	rm -f *.o
