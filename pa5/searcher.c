#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <limits.h> /*for PATH.MAX"*/
#include "tokenizer.h"
#include "hash.h"
#include "searcher.h"
#include "cache.h"

/*
loads file into memory(string) and then calls
on methods to tokenize a file list and term hashtable
Returns number of files described in file
*/
int Hash_filescan(char *file_name, char **files, hashTable tbl)
{
	FILE * fileptr;
	if ((fileptr = fopen(file_name, "r")) == NULL) {
		printf("File Not Found, and Thus Skipped: %s\n", file_name);
		return 0;
	}

	fseek(fileptr, 0L, SEEK_END);
	long lSIZE = ftell(fileptr);
	fseek(fileptr, 0L, SEEK_SET);

	char * buffer;
	if((buffer = (char *)calloc(lSIZE+1, sizeof(char))) == 0)
	{
		printf("Not enough memory");
		return 0;
	}

	fread(buffer, sizeof(char), lSIZE, fileptr);
	fclose(fileptr);

	TokenizerT tokenizer = TKCreate("<>\n", buffer);
	char * tok = NULL; 
	tok = TKGetNextToken(tokenizer);
	int file_nums = atoi(tok);

	tok = TKGetNextToken(tokenizer);
	int term_num = atoi(tok);

	printf("# Files: %d\n# Words: %d\n", file_nums, term_num);

	files = buildFileList(tokenizer, file_nums);
	if(files == NULL)
	{
		printf("File List is NULL");
		return 0;
	}
	/*
	tbl = buildHash(tokenizer, term_num, file_nums, NULL);

	if(tbl == NULL)
	{
		printf("No terms in file");
		return 0;
	}
	print_Hash(tbl);
	*/
	return file_nums;	
}

/*
Builds file array that contains file names
*/
char** buildFileList(TokenizerT tokenizer, int file_nums)
{
	if(file_nums < 1)
	{
		return NULL;
	}

	char ** file_list;

	if( (file_list = (char**)calloc(file_nums, sizeof(char*))) == NULL)
	{
		printf("No memory");
		return NULL;
	}

	char * current_tok = NULL;
	current_tok = TKGetNextToken(tokenizer);
	int index = 0;
	while(strcasecmp(current_tok, "/files") != 0)
	{
		
		if(strcasecmp(current_tok, "name") == 0)
		{
			free(current_tok);
			current_tok = TKGetNextToken(tokenizer);
			if((file_list[index] = (char*)calloc(strlen(current_tok)+1, sizeof(char))) == NULL)
			{
				printf("No memory\n");
				return NULL;
			}
			strcpy(file_list[index], current_tok);
			index++;
		}
		free(current_tok);
		current_tok = TKGetNextToken(tokenizer);
	}
	free(current_tok);
	return file_list;
}

/*
Creates the hashTable and inserts the terms along with there file appearances into 
it.
*/
hashTable buildHash(TokenizerT tokenizer, int term_num, int file_nums, Limits limit)
{
	hashTable tbl = create_HashTable(term_num);
	unsigned long int tot_mem = 0, cur_mem = 0;
	char* current_tok;
	current_tok = TKGetNextToken(tokenizer);

	while(current_tok != NULL)
	{
		/*term is next, store it into the hash*/
		if(strcasecmp(current_tok, "list") == 0) 
		{
			free(current_tok);

			char * term;
			term = TKGetNextToken(tokenizer);

			int * files;
			if ((files = (int *)calloc(file_nums, sizeof(int))) == NULL)
			{
				printf("Not enough Memory\n");
				return NULL;
			}

			memset(files, 0, file_nums);
			char * file_frequency = TKGetNextToken(tokenizer);
			cur_mem += mem_count(term, file_nums);
			tot_mem += cur_mem;

			while(strcasecmp(file_frequency, "/list") != 0)
			{
				int file_index = atoi(file_frequency);
				free(file_frequency);

				file_frequency = TKGetNextToken(tokenizer);
				int frequency = atoi(file_frequency);
				files[file_index] = frequency;
				free(file_frequency);
				file_frequency = TKGetNextToken(tokenizer);
			}
			free(file_frequency);

			if (limit->memory_limit != -1 &&
				tot_mem > limit->memory_limit) {
				free(files);
				free(term);
				break;
			}
			/*printf("[%lu] %s <#char>%lu <#int>%lu\n", cur_mem, term, strlen(term)*sizeof(char), ints*sizeof(int));*/
			
			insert_Hash(tbl, term, files, file_nums);
			cur_mem = 0;
			free(files);
			free(term);
			/*copy files to new int array*/
		}
		current_tok = TKGetNextToken(tokenizer);
	}
	return tbl;
}

/*
Returns indexes of files that contain all terms
*/
int * Search_And(int file_count, hashTable tbl, hashTable loc, FILE *fileptr, TokenizerT tokenizer)
{
	int * files;
	if( (files = (int*)calloc(file_count, sizeof(int))) == 0)
	{
		printf("Not enough memory\n");
		return NULL;
	}
	memset(files, 0, file_count);

	char * current_tok;
	current_tok = TKGetNextToken(tokenizer);
	int init = 0; int counter = 0;
	int i; /*for loop var*/
	while(current_tok != NULL && strcasecmp(current_tok, "") != 0) 
	{
		int * ptr;
		ptr = search_Hash(tbl, current_tok);
		/* If the term doesn't exist in cache, look in file */
		if(ptr == NULL)
		{
			printf("Searching through file for \"%s\"\n", current_tok);
			ptr = search_Hash(loc, current_tok);
			/* If term doesn't exist in file either */
			if (ptr == NULL) {
				printf("Term doesn't exist: %s\n", current_tok);
				return NULL;
			} else {
				ptr = get_address(fileptr, ptr, file_count);
				/* Attempts to insert the term into the cache */
				insert_Hash(tbl, current_tok, ptr, file_count);
			}
		}
		if(init == 0) /*copy value of first file array into empty answer array*/
		{
			for(i = 0; i < file_count; i++)
			{
				files[i] = ptr[i];
				if(files[i] != 0)
				{
					counter++;
				}
			}
			init = 1;
		}	
		else /*answer array is no longer empty*/
		{
			for(i = 0; i < file_count; i++)
			{
				if((files[i] != 0 && ptr[i] == 0)) /*if there's no frequency count*/
				{
					files[i] = 0;
					counter--;
				}
			}
		}
		free(current_tok);
		current_tok = TKGetNextToken(tokenizer);
	}
	free(current_tok);
	if(counter == 0)
	{
		return NULL;
	}

	return files;
}

/* 
Returns indexes of files that contain at least one term
*/
int * Search_Or(int file_count, hashTable tbl, hashTable loc, FILE *fileptr, TokenizerT tokenizer)
{
	int * files;
	if( (files = (int*)calloc(file_count, sizeof(int))) == 0)
	{
		printf("Not enough memory");
		return NULL;
	}
	memset(files, 0, file_count);

	char * current_tok;
	current_tok = TKGetNextToken(tokenizer);
	int init = 0;
	int i; /*for loop var*/
	while(current_tok != NULL && strcasecmp(current_tok, "") != 0)
	{
		int * ptr;
		ptr = search_Hash(tbl, current_tok);
		/* If the term doesn't exist in cache, look in file */
		if(ptr == NULL)
		{
			printf("Searching through file for \"%s\"\n", current_tok);
			ptr = search_Hash(loc, current_tok);
			/* If term doesn't exist in file either */
			if (ptr == NULL) {
				printf("Term doesn't exist: %s\n", current_tok);
				free(current_tok);
				current_tok = TKGetNextToken(tokenizer);
				continue;
			} else {
				ptr = get_address(fileptr, ptr, file_count);
				/* Attempts to insert the term into the cache */
				insert_Hash(tbl, current_tok, ptr, file_count);
			}
		}
		if(init == 0) /*copy value of first file array into empty answer array*/
		{
			for(i = 0; i < file_count; i++)
			{
				files[i] = ptr[i];
			}
			init = 1;
		}	
		else /*answer array is no longer empty*/
		{
			for(i = 0; i < file_count; i++)
			{
				if((files[i] == 0 && ptr[i] != 0)) /*if there's no frequency count*/
				{
					files[i] = ptr[i];
				}
			}
		}
		free(current_tok);
		current_tok = TKGetNextToken(tokenizer);
	}
	free(current_tok);
	return files;
}
