#ifndef CACHE_H_
#define CACHE_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include "hash.h"


struct limit {
	unsigned long int memory_limit;
};
typedef struct limit *Limits;

char *substring(int start, int end, char *word);

unsigned long int calc_memory(char *limit);

hashTable filter(FILE *fileptr, int term_num);

#endif
