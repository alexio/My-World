#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <limits.h> /*for PATH.MAX"*/
#include "tokenizer.h"
#include "hash.h"
#include "searcher.h"

/*No spaces between <list> and term in indexer output file*/

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
		/*
    char * line = malloc(100), * linep = line;
    size_t lenmax = 100, len = lenmax;
    int c;

    if(line == NULL)
        return NULL;

    while(1){

        c = fgetc(stdin);
        if(c == EOF)
            break;

        if(--len == 0) {
            char * linen = realloc(linep, lenmax *= 2);
            len = lenmax;

            if(linen == NULL) {
                free(linep);
                return NULL;
            }
            line = linen + (line - linep);
            linep = linen;
        }

        if((*line++ = c) == '\n')
            break;
    }
    *line = '\0';
    return linep;*/
}

int main(int argc, char ** argv)
{
	if (argc != 2) {
		printf("Insufficient arguments\n");
		return 0;
	}

	char * file_name = argv[1];

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

	hashTable tbl = NULL;
	char **files = NULL;
	files = buildFileList(tokenizer, file_nums);
	if(files == NULL)
	{
		printf("File List is NULL");
		return 0;
	}

	tbl = buildHash(tokenizer, term_num, file_nums);

	if(tbl == NULL)
	{
		printf("No terms in file");
		return 0;
	}

	print_Hash(tbl);

	/*
	int file_count = Hash_filescan(argv[1], files, tbl);*/


	char * input;
	char * option;
	/* User Interaction **/
	
  	while (1) 
	{
		
		printf("Search Options(Case Sensitive):  :\n");
		printf("sa: Return only files that contain all terms in the query\n");
		printf("so: return any file that contains any subset of the terms in the query\n");
		printf("q : quit\n");
		printf("Type in your search option followed by search terms:");
		
		input = readStdin(); 
		
		
		printf("input: %s\n", input);
		TokenizerT tokenizer = TKCreate(" ", input);
		option = TKGetNextToken(tokenizer);
		printf("Option: %s\n", option);

		if (strcasecmp(option, "sa") == 0)
		 {
			int * answer;
			answer = searchand(file_count, tbl, tokenizer);
			if (answer == NULL)
			{
				printf("\n");
				printf(" No file contained all the terms you desired or no search terms were inputed\n");
			}
			else
			{
				/* print it all and free */
				int i;
				for(i = 0; i < file_count; i++)
				{
					if(answer[i] != 0)
					{
						printf("%s\n", files[i]);
					}
				}
			}
		}	
		
		else if (strcasecmp(option, "so") == 0)
		{

			
			int any = 0;
			int * searchAns;
			searchAns = searchor(file_count, tbl, tokenizer);

			int i;
			for(i = 0; i < file_count; i++){
				
				/*
				loop and print out the filekey if it's not equal to 0*
				print the files which contain the search terms inputed 
				by the user, searchAns is a int * array which is the 
				size of the filekeys array and is used to check if 
				the word is in the file with the corresponding indexes 
				in searchAns and filekeys, 1= yes , 0 = no */
				
				
				if(searchAns[i] != 0){
					any = 1;
					printf("%s\n", files[i]);
				}
			}
			if (any == 0) {
				printf("Sorry, there were no files found that had the terms you asked for.\n");
			}
			
		}
		else if (strcasecmp(option, "q") == 0) {
			
			/* quit the program somehow, free all memory 
			free(tk);
			freetable(hashmap);*/
			return 0;
		}
		else {
			
			printf("You've entered an incorrect option/format, please try again\n");
		}
		printf("\n");
	}
	

	return 0;
}


