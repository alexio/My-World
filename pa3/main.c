
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include "indexer.h"
#include "hash.h"

int main(int argc, char ** argv) {
	
	/*
	if (argc < 3) {
		printf("Insufficient arguments\n");
		return 0;
	}

	File *file1;
	hashTable tbl = create_HashTable(100);
	struct stat statbuf;
	stat(argv[2], &statbuf);

	if(S_ISDIR(statbuf.st_mode))
	{
		recurseDir(tbl, argv[2]);
	}
	else
	{
		filescan(tbl, argv[2]);
	}
	*/
	
	hashTable tbl = create_HashTable(100);
	filescan(tbl, argv[1]);

	return 0;
}
