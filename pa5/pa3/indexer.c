#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <limits.h> /*for PATH.MAX"*/
#include "indexer.h"
#include "tokenizer.h"
#include "tree.h"

#define maxline 200

void recurseDir(Tree tbl, char *dir_name, Files filenames, Counter counter)
{
	DIR * direct;
	direct = opendir(dir_name);

	if(! direct)
	{
		printf("Cannot open directory: %s\n", dir_name);
		exit(EXIT_FAILURE);
	}
	struct dirent * dir;
	const char * d_name;

	while((dir = readdir(direct)) != NULL)
	{
		d_name = dir->d_name; /*Get name of next director/file*/
		/*printf("Directory or file: %s\n", dir_name);
		printf("dname %s\n", d_name);*/

		
		/*
		 * Checks to see if it's trying to call it on the current or parent directory,
		 *  if current, recursively call the same method,
		 *  otherwise, calls the filescan() method
		 */
		if (strcasecmp(d_name, ".") != 0 
		&& strcasecmp(d_name, "..") != 0)
		{ 
			char * next_path; /*path for file or directory*/

			if((next_path = calloc(strlen(d_name) + strlen(dir_name) + 2, sizeof(char))) == 0)
			{
				printf("Memory allocation unsuccessful\n");
				exit(1);
			}

			strcpy(next_path, dir_name);
			strcat(next_path, "/");
			strcat(next_path, d_name);
			strcat(next_path, "\0");

			struct stat statbuf;
			stat(next_path, &statbuf);

			if(S_ISDIR(statbuf.st_mode)) {
				recurseDir(tbl,next_path, filenames, counter);
			} else {
				filescan(tbl,next_path, filenames, counter);
			}
		}
	}

	if (closedir(direct)) {
		printf("Failed to close Direct: %s\n", dir_name);
        exit (EXIT_FAILURE);
    }
}

/*
 * The method filescan() will attempt to read the given file,
 *  grab each line from the file via fgets(),
 *  tokenizes each line via TKCreate() & TKGetNextToken(),
 *  then insert each tokens into the hash table via insert_hash()
 */
void filescan(Tree tvl, char* file_name, Files filenames, Counter counter)
{
	FILE * fileptr;
	/* attempts to open the file */
	if ((fileptr = fopen(file_name, "r")) == NULL) {
		printf("File Not Found, and Thus Skipped: %s\n", file_name);
		return;
	}

	counter->file_count++;

	if (filenames->name == NULL) {
		filenames->name = file_name;
		/*printf("file1: %s\n", filenames->name);*/
	} else {
		Files file_ptr = filenames;
		while (file_ptr->next != NULL) {
			file_ptr = file_ptr->next;
		}

		Files newfile = calloc(1, sizeof(struct filename));
		newfile->name = file_name;
		newfile->next = NULL;
		file_ptr->next = newfile;
		/*printf("file2: %s\n", file_ptr->next->name);*/
	}
	char line[maxline];
	/* attempts to grab each line in the file*/
	while (fgets(line, sizeof line, fileptr) != NULL) {
		TokenizerT tokenizer = TKCreate(" ~!@#$%^&*()_+`<>?[]{};':\",./\\n", line);
		char* token = NULL;
		/* attempts to tokenizes the token stream*/
		while (tokenizer->position < strlen(tokenizer->stream)) {
			token = TKGetNextToken(tokenizer);
			
			if (token != NULL) {
				int i;
				for(i = 0; token[i]; i++){
	 				 token[i] = tolower(token[i]);
				}
				insert_Tree(tvl, token, file_name, counter);
			}
			tokenizer->position++;
			free(token);
		}
		TKDestroy(tokenizer);
	};
	fclose(fileptr);
}

	