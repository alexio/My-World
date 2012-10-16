#ifndef INDEXER_H_
#define INDEXER_H_

#include <stdlib.h>
#include <stdio.h>
#include "hash.h"

void recurseDir(hashTable tbl, char* dir_name);
void filescan(hashTable tvl, char* file_name);

#endif