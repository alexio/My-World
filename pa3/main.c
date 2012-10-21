
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include "indexer.h"
#include "hash.h"

#define hash_size 1

int main(int argc, char ** argv) {
	
	if (argc < 3) {
		printf("Insufficient arguments\n");
		return 0;
	}

	printf("File 1 %s, File 2 %s \n", argv[1], argv[2]);
	
	hashTable tbl = create_HashTable(hash_size);
	struct stat statbuf;
	/* for directory check*/
	stat(argv[2], &statbuf);

	/*checks to see if it's a directory*/
	if(S_ISDIR(statbuf.st_mode)) {
		printf("Goes Here Dir\n");
		recurseDir(tbl, argv[2]);
	} else {
		printf("Goes Here\n");
		filescan(tbl, argv[2]);
	}

	write(argv[1], tbl);
	
	return 0;
}
