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
#include "hash.h"
#include "tree.h"

#define maxline 200

void recurseDir(Tree tbl, char * dir_name)
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
		printf("Directory or file: %s\n", dir_name);
		printf("dname %s\n", d_name);

		
		/*checks to see if it's trying to call it on the current or parent directory again
		, so just stops it.*/
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
			/*
			 *
			 *
			strcat(next_path, '\0');
			 *
			 *
			 */
			 strcat(next_path, "\0");

			struct stat statbuf;
			stat(next_path, &statbuf);

			if(S_ISDIR(statbuf.st_mode)) {
				recurseDir(tbl,next_path);
			} else {
				filescan(tbl,next_path);
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
void filescan(Tree tvl, char* file_name)
{
	FILE * fileptr;
	/* attempts to open the file */
	if ((fileptr = fopen(file_name, "r")) == NULL) {
		printf("File Not Found, and Thus Skipped: %s\n", file_name);
		return;
	}

	char line[maxline];
	/* attempts to grab each line in the file*/
	while (fgets(line, sizeof line, fileptr) != NULL) {
		TokenizerT tokenizer = TKCreate(" ~!@#$%^&*()_+`<>?[]{};':\",./\\n", line);
		char* token = NULL;
		/* attempts to tokenizes the token stream*/
		while (tokenizer->position < strlen(tokenizer->tokenPTR)) {
			token = TKGetNextToken(tokenizer);
			
			if (token != NULL) {
				
				int i;
				for(i = 0; token[i]; i++){
	 				 token[i] = tolower(token[i]);
				}
				insert_Tree(tvl, token, file_name);
			}
			tokenizer->position++;
			free(token);
		}
		TKDestroy(tokenizer);
	}
	;
	fclose(fileptr);
}

/*
 * 
 */
/*int write(char *filename, hashTable tbl) {

	int i;
	hashNode h_ptr;
	docNode f_ptr;
	FILE *file;

	h_ptr = NULL;
	f_ptr = NULL;
	file = fopen(filename, "a+");

	for(i = 0; i < tbl->size; i++)
	{
		h_ptr = tbl->Htable[i];
		while(h_ptr != NULL)
		{
			printf(" Term %s\n", h_ptr->term);
			fprintf(file, "<list> %s\n", h_ptr->term);
			f_ptr = h_ptr->files_with_term;
			while(f_ptr != NULL)
			{
				printf(" File: %s \n", f_ptr->file_name);
				printf("Frequency %d \n", f_ptr->frequency);
				fprintf(file, "%s %d", f_ptr->file_name, f_ptr->frequency);
				f_ptr = f_ptr->next;
			}
			fprintf(file, "\n</list>\n");
			h_ptr = h_ptr->next;
		}
	}
}*/