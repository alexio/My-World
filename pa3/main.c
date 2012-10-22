
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include "indexer.h"
#include "tree.h"

#define hash_size 1

int main(int argc, char ** argv) {
	
	if (argc < 3) {
		printf("Insufficient arguments\n");
		return 0;
	}

	printf("File 1 %s, File 2 %s \n", argv[1], argv[2]);
	
	/*hashTable tbl = create_HashTable(hash_size);*/

	Tree branch = treeCreate();
	struct stat statbuf;
	/* for directory check*/
	stat(argv[2], &statbuf);

	/*checks to see if it's a directory*/
	if(S_ISDIR(statbuf.st_mode)) {
		
		recurseDir(branch, argv[2]);
	} else {
		filescan(branch, argv[2]);
	}

	/*write(argv[1], tbl);*/
	FILE* ptr = fopen(argv[1], "a+");
	print(branch, ptr);
	fclose(ptr);
	return 0;
}
