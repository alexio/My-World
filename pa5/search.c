#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <strings.h>
#include <string.h>
#include <errno.h>
#include "tokenizer.h"
#include "hash.h"
#include "searcher.h"
#include "cache.h"

/* No spaces between <list> and term in indexer output file */
char * readStdin() {

	char * input = calloc(500, sizeof(char));
	char ch = getchar();
	int char_count = 0;
	while( (ch != '\n')  &&  (char_count < 500)) 
	{
		input[char_count++] = ch;
		ch = getchar();
	}
	input[char_count] = 0x00; 

	return input;
}

int main(int argc, char ** argv) {

	char *file_name;
	unsigned long int memory_limit = 0;

	if (argc == 2) {
		file_name = argv[1];
		memory_limit = -1;
	} else if (argc == 4) {
		file_name = argv[3];
		memory_limit = calc_memory(argv[2]);
	} else {
		printf("Insufficient arguments\n");
		return 0;
	}

	printf("======================\n");
	printf("Memory Limit: %lu\n", memory_limit);
	printf("Inverted File: %s\n", file_name);
	printf("======================\n");

	Limits limit = calloc(1, sizeof(struct limit));
	limit->memory_limit = memory_limit;
	hashTable loc = NULL;

	FILE * fileptr;
	if ((fileptr = fopen(file_name, "r")) == NULL) {
		printf("File Not Found, and Thus Skipped: %s\n", argv[1]);
		return 0;
	}

	fseek(fileptr, 0L, SEEK_END);
	long lSIZE = ftell(fileptr);
	fseek(fileptr, 0L, SEEK_SET);

	char * buffer;
	if((buffer = (char *)calloc(lSIZE+1, sizeof(char))) == 0) {
		printf("Not enough memory");
		return 0;
	}

	fread(buffer, sizeof(char), lSIZE, fileptr);

	TokenizerT tokenizer = TKCreate(" <>\n", buffer);

	free(buffer); /**/

	char * tok = NULL; 
	tok = TKGetNextToken(tokenizer);
	int file_nums = atoi(tok);
	free(tok);

	tok = TKGetNextToken(tokenizer);
	int term_num = atoi(tok);
	free(tok);

	loc = filter(fileptr, term_num);
	fclose(fileptr);

	hashTable tbl = NULL;
	char **files = NULL;
	files = buildFileList(tokenizer, file_nums);

	if(files == NULL) {
		printf("File list is NULL");
		return 0;
	}

	tbl = buildHash(tokenizer, term_num, file_nums, limit);
	
	if(tbl == NULL) {
		printf("No terms in file");
		return 0;
	}
	/*
	int file_count = Hash_filescan(argv[1], files, tbl);*/

	char * input;
	char * option;
	/* User Interaction **/
	
  	while (1) {

		printf("Search Options(Case Sensitive):\n");
		printf("sa: Return only files that contain all terms in the query\n");
		printf("so: return any file that contains any subset of the terms in the query\n");
		printf("q : quit\n");
		printf("Type in your search option followed by search terms:");
		
		input = readStdin(); 
		printf("\n");
		TokenizerT tokenizer2 = TKCreate(" ", input);
		option = TKGetNextToken(tokenizer2);
		
		if(option != NULL)
		{
			if (strcasecmp(option, "sa") == 0)
			 {
				int * answer;
				answer = Search_And(file_nums, tbl, tokenizer2);
				if (answer == NULL)
				{
					printf("\n");
					printf(" No file contained all the terms you desired or no search terms were inputed\n");
				}
				else
				{
					printf("Files containing all the terms: \n");
					/* print it all and free */
					int i;
					for(i = 0; i < file_nums; i++)
					{
						if(answer[i] != 0)
						{
							printf("File -> %s\n", files[i]);
						}
					}
				}
			}	
			
			else if (strcasecmp(option, "so") == 0)
			{

				
				int any = 0;
				int * searchAns;
				searchAns = Search_Or(file_nums, tbl, tokenizer2);
				
				printf("Files containing at least one term:\n");
				int i;
				for(i = 0; i < file_nums; i++){
					
					/*
					loop and print out the filekey if it's not equal to 0*
					print the files which contain the search terms inputed 
					by the user, searchAns is a int * array which is the 
					size of the filekeys array and is used to check if 
					the word is in the file with the corresponding indexes 
					in searchAns and filekeys, 1= yes , 0 = no */
					
					if(searchAns[i] != 0){
						any = 1;
						printf("File -> %s\n", files[i]);
					}
				}
				if (any == 0) {
					printf("Sorry, there were no files found that had the terms you asked for.\n");
				}
				
			}
			else if (strcasecmp(option, "q") == 0) {
				
				/* quit the program somehow, free all memory */
				TKDestroy(tokenizer2);
				free(option);
				free(input);
				TKDestroy(tokenizer);
				int j;
				for(j = 0; j < file_nums ; j++)
				{
					free(files[j]);
				}
				free(files);
				free(limit);
				
				destroy_HashTable(tbl);
				destroy_HashTable(loc);
				return 0;
			}
			else {
				
				printf("You've entered an incorrect option/format, please try again\n");
			}
		}
		else {
				
				printf("You've entered an incorrect option/format, please try again\n");
		}
		TKDestroy(tokenizer2);
		free(option);
		free(input);
		printf("\n");
	}

	return 0;
}


