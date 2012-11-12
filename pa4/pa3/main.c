
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

	Counter counter;
	if ((counter = calloc(1, sizeof(struct counter))) == 0) {
		printf("Out of memory @ main.c line 27\n");
		return 0;
	}
	counter->word_count = 0;
	counter->file_count = 0;

	Files filenames;
	if ((filenames = calloc(1, sizeof(struct filename))) == 0) {
		printf("Out of memory @ main.c line 35\n");
		return 0;
	}
	filenames->name = NULL;
	filenames->next = NULL;

	struct stat statbuf;
	/* for directory check*/
	stat(argv[2], &statbuf);

	/*checks to see if it's a directory*/
	if(S_ISDIR(statbuf.st_mode)) {
		recurseDir(branch, argv[2], filenames, counter);
	} else {
		filescan(branch, argv[2], filenames, counter);
	}

	/*write(argv[1], tbl);*/
	FILE* ptr = fopen(argv[1], "a+");

	fprintf(ptr, "<%i>\n", counter->file_count);
	fprintf(ptr, "<%i>\n", counter->word_count);

	print_file(ptr, filenames);

	print(branch, ptr, filenames);

	//printf("File counts: %i\nWord counts: %i\n", counter->file_count, counter->word_count);
	fclose(ptr);
	return 0;
}
