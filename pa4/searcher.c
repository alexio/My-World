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

/*
loads file into memory(string) and then calls
on methods to tokenize a file list and term hashtable
*/
hashTable Hash_filescan(string filename, char** files, hashTable tbl)
{
	FILE * fileptr;
	if ((fileptr = fopen(file_name, "r")) == NULL) {
		printf("File Not Found, and Thus Skipped: %s\n", file_name);
		return NULL;
	}

	fseek(fileptr, 0L, SEEK_END);
	long lSIZE = ftell(fileptr);
	fseek(fileptr, 0L, SEEK_SET);

	char * buffer;
	if((buffer = (char *)calloc(lSIZE+1, sizeof(char))) == 0)
	{
		printf("Not enough memory");
		return NULL;
	}

	fread(buffer, sizeof(char), lSIZE, fileptr);
	fclose(fileptr);

	TokenizerT tokenizer = TKCreate("<>", buffer);
	char * tok = NULL; 
	tok = TKGetNextToken(tokenizer);
	int file_nums = atoi(tok);

	tok = TKGetNextToken(tokenizer);
	int term_num = atoi(tok);

	printf("Numbers %d  %d\n", file_nums, term_num);

	files = buildFileList(tokenizer, file_nums);
	if(files == NULL)
	{
		printf("File List is NULL");
	}

	tbl = buildHash(tokenizer, term_num, file_nums);

	return file_nums;	
}

/*
Builds file array that contains file names
*/
char ** buildFileList(TokenizerT tokenizer, int file_nums)
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

	char * current_tok == NULL;
	current_tok = TKGetNextToken(tokenizer);
	int index = 0;
	while(strcasecmp(current_tok, "/files") != 0)
	{
		
		if(strcasecmp(current_tok, "name") == 0
		 	|| strcasecmp(current_tok, "/name") == 0 
			|| strcasecmp(current_tok, "files") == 0)
		{
			if((file_list[index] = (char*)calloc(strlen(current_tok), sizeof(char))) == NULL)
			{
				printf("No memory\n");
				return NULL;
			}
			strcpy(file_list[index], current_tok);
			index++;
		}
		current_tok = TKGetNextToken(tokenizer);
	}


	return file_list;
}

/*
Creates the hashTable and inserts the terms along with there file appearances into 
it.
*/
hashTable buildHash(TokenizerT tokenizer, int term_num, int file_nums)
{

	hashTable tbl = create_HashTable(term_num);

	char* current_tok;
	current_tok = TKGetNextToken(tokenizer);

	while(current_tok != NULL)
	{
		/*term is next, store it into the hash*/
		if(strcasecmp(current_tok, "list") == 0) 
		{
			free(current);

			char * term;
			term = TKGetNextToken(tokenizer);

			int * files;
			if( (files = (int *)calloc(file_nums, sizeof(int))) == NULL)
			{
				printf("Not enough Memory\n");
				return NULL;
			}

			memset(files, 0, file_nums);

			current_tok = TKGetNextToken(tokenizer);
			while(strcasecmp(current, "/list") != 0)
			{
				char * file_frequency = strtok(current_tok, " ");
				int file_index = atoi(file_frequency);

				file_frequency = strtok(NULL, "<>");
				int frequency = atoi(file_frequency);
				files[file_index] = frequency;

				free(current);
				current_tok = TKGetNextToken(tokenizer);
			}
			insert_Hash(tbl, term, files, file_nums);
			free(term);
			free(current)
			/*copy files to new int array*/
		}
		current_tok = TKGetNextToken(tokenizer);
	}
}

/*
Returns indexes of files that contain all terms
*/
int * Search_And(int file_count, hashTable tbl, tokenizer)
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
		if(ptr == NULL)
		{
			printf("Term ,%s\n ,not present and thus skipped", current_tok);
		}
		else
		{
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
					if((files[i] != 0 && ptr[i] == 0)) /*if there's no frequency count*/
					{
						files[i] = 0;
					}
				}

			}
			current_tok = TKGetNextToken(tokenizer);
		}
	}
	return files;
}

/* 
Returns indexes of files that contain at least one term
*/
int * Search_Or(int file_count, hashTable tbl, tokenizer)
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
		if(ptr == NULL)
		{
			printf("Term ,%s\n ,not present and thus skipped", current_tok);
		}
		else
		{
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
			current_tok = TKGetNextToken(tokenizer);
		}
	}
	return files;
}
