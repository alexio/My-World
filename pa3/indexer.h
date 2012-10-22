#ifndef INDEXER_H_
#define INDEXER_H_

#include <stdlib.h>
#include <stdio.h>
#include "tree.h"

void recurseDir(Tree tbl, char* dir_name);
void filescan(Tree tvl, char* file_name);

#endif