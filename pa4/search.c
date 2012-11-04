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
    return linep;
}

int main(int argc, char ** argv)
{
	if (argc != 2) {
		printf("Insufficient arguments\n");
		return 0;
	}


	hashTable tbl = NULL;
	char ** files = NULL;

	int file_count = Hash_filescan(argv[1], files, tbl);

	print_Hash(tbl);

	printf("Count: %d\n", file_count);

	
	/*char * input;
	char * option;
	 User Interaction */ 

  	/*
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
				print it all and free
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
		
		else if (strcasecmp(option, "so") == 0) {
			
			int any = 0;
			int * searchAns;
			searchAns = searchor(file_count, tbl, tokenizer);

			int i;
			for(i = 0; i < number; i++){
				loop and print out the filekey if it's not equal to 0*
				print the files which contain the search terms inputed 
				by the user, searchAns is a int * array which is the 
				size of the filekeys array and is used to check if 
				the word is in the file with the corresponding indexes 
				in searchAns and filekeys, 1= yes , 0 = no 
				
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
			
			quit the program somehow, free all memory
			free(tk);
			freetable(hashmap);
			return 0;
		}
		else {
			
			printf("You've entered an incorrect option/format, please try again\n");
		}
		printf("\n");
		free(tk1);
	}*/

	return 0;
}


