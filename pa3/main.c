
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include "index.h"
#include "hash.h"

int main(int argc, char ** argv) {
	
	if (argc < 3) {
		printf("Insufficient arguments\n");
		return 0;
	}

	File * file1;
	hashTable tbl = create_HashTable(100);
	struct stat statbuf;

	stat(argv[2], &statbuf);

	if(S_ISDIR(statbuf.st_mode))
	{
		recurseDir(tbl, direc);
	}
	else
	{
		filescan(tbl, direc);
	}

	return 0;
}
