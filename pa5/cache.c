#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "cache.h"
#include "hash.h"
#include "tokenizer.h"

/*
 * Attempts to take the substring of a given string,
 *  from the given start to the given end.
 */
char* substring(int start, int end, char *word) {
	char *substring = calloc(end-start+1, sizeof(char));
	int i = start, n = 0;
	for (; i < end; i++, n++) {
		substring[n] = word[i];
	}
	substring[n] = '\0';
	return substring;
}

/*
 * Attempts to calculate the given memory limit to bytes,
 *  checks if the given memory limit is over 1KB.
 *
 * Returns -1 if the input is incorrect, otherwise the calculated byte amount.
 */
unsigned long int calc_memory(char *limit) {
	int length = strlen(limit);
	char * output = substring(0, length-2, limit);
	unsigned long int bytes = atoi(output);
	free(output);
	char *unit = substring(length-2, length, limit);
	if (strcmp(unit, "KB") == 0) {
		bytes = bytes * 1024;
	} else if (strcmp(unit, "MB") == 0) {
		bytes = bytes * 1024 * 1024;
	} else if (strcmp(unit, "GB") == 0) {
		bytes = bytes * 1024 * 1024 * 1024;
	} else {
		bytes = atoi(limit);
	}
	free(unit);
	return bytes;
}

/*
 * Attempts to print out the hash table,
 *  outputs the word and its respected byte address in the file
 */
void print_Cache(hashTable loc) {	
	int i, count;
	hashNode h_ptr = NULL;
	for (i = 0, count = 0; i < loc->size; i++)
	{
		count++;
		h_ptr = loc->Htable[i];
		while (h_ptr != NULL) {
			printf("[Term %i] \"%s\" @ %i\n", count, h_ptr->term, h_ptr->files[0]);
			h_ptr = h_ptr->next;
		}
	}
}

/*
 * Attempts to jump to the given byte address in the given file,
 *  reads the line that contains the file number and its corresponding frequency,
 *  stores the information in an int array and returns it to the caller.
 *
 * Will fail if the byte address is incorrect or out of memory.
 */
int* get_address(FILE *fileptr, int *byte_address, int file_nums) {
	int *files, size = 50, length;
	int index, frequency;
	char *line, *token;
	TokenizerT tokenizer;
	if ((files = (int *)calloc(file_nums, sizeof(int))) == NULL ||
		(line = calloc(size, sizeof(char))) == NULL) {
		printf("Not enough memory in cache.c @ get_address().1\n");
		return NULL;
	}
	memset(files, 0, file_nums);
	fseek(fileptr, byte_address[0], SEEK_SET);
	fgets(line, size, fileptr);
	/* Trims off the "\n" */
	length = strlen(line);
	line[length-1] = '\0';

	tokenizer = TKCreate(" <>\n", line);
	while ((token = TKGetNextToken(tokenizer)) != NULL) {
		index = atoi(token);
		free(token);
		/*printf("<%i ", index);*/
		token = TKGetNextToken(tokenizer);
		frequency = atoi(token);
		/*printf("%i>", frequency);*/
		free(token);
		files[index] = frequency;
	}
	/*printf("\n");*/
	free(line);
	free(token);
	TKDestroy(tokenizer);
	return files;
}

/* 
 * Calculate the memory size for storing the term,
 *  and the frequencies with the respective files.
 */
int mem_count(char *term, int file_nums) {
	int term_mem = sizeof(char) * strlen(term);
	int files_mem = sizeof(int) * file_nums;
	return term_mem + files_mem;
}

/*
 * Attempts to store the word and its respected byte address
 *  into the given hash table object.
 */
hashTable insert_Cache(hashTable table, char *input, int *file_array) {
	if (input == NULL || table == NULL) {
		printf("Hash table and/or input is empty!\n");
		return 0;
	}

	int index = hash_Function(table->size, input);

	int *files;
	if ((files = (int *)calloc(1, sizeof(int))) == NULL) {
		printf("Not enough memory in cache.c @ insert_Cache().1\n");
		return 0;
	}

	memset(files, 0, 1);
	files[0] = file_array[0];
	
	hashNode new;
	if((new = calloc(1, sizeof(struct hashNode))) == 0) {
		return 0;
	}

	new->term = calloc(strlen(input)+1, sizeof(char)); 
	strcpy(new->term, input);
	new->files = files;

	if(table->Htable[index] == NULL) {
		table->Htable[index] = new;
	} else if(strcmp(table->Htable[index]->term, input) == 0) {
		/* Does nothing because the terms are the same */
	} else if(strcmp(table->Htable[index]->term, input) > 0) {
		new->next = table->Htable[index];
		table->Htable[index] = new;
	} else {
		hashNode ptr = table->Htable[index];
		while(ptr->next != NULL) {
			if(strcmp(ptr->next->term, input) == 0) {
				/* Does nothing because the terms are the same */
			}
			else if(strcmp(ptr->next->term, input) > 0) {
				new->next = ptr->next;
				ptr->next = new;
			}
			ptr = ptr->next;
		}
		ptr->next = new;
	}
	return table;
}

/*
 * Attempts to read the given file and store the words' byte addresses
 */
hashTable filter(FILE *fileptr, int term_num) {
	char *term, *token;
	int *bytes, size = 50, termsize = 8, count = 0;
	fseek(fileptr, 0L, SEEK_SET);
	hashTable loc = create_HashTable(term_num);

	if ((term = calloc(termsize, sizeof(char))) == NULL) {
		printf("Not enough memory in cache.c @ filter().1\n");
		return NULL;
	}

	while (fgets(term, termsize, fileptr) != NULL) {
		/*printf("<%i> %s\n", count, term);*/
		if (strcmp(term, "<list> ") == 0) {
			if ((bytes = (int *)calloc(1, sizeof(int))) == NULL ||
				(token = calloc(size, sizeof(char))) == NULL) {
				printf("Not enough memory in cache.c @ filter().2 \n");
				return NULL;
			}
			memset(bytes, 0, 1);
			fgets(token, size, fileptr);
			int length = strlen(token);
			/* Trims off the "\n" */
			token[length-1] = '\0';
			/*printf("\"%s\"\n", token);*/
			bytes[0] = ftell(fileptr);
			count++;
			/*printf("[Term %i] %s @ %i:", count, token, bytes[0]);
			get_address(fileptr, bytes, 1);*/
			loc = insert_Cache(loc, token, bytes);
			free(bytes);
			free(token);
		}
		free(term);
		if ((term = calloc(termsize, sizeof(char))) == NULL) {
			printf("Not enough memory in cache.c @ filter().3\n");
			return NULL;
		}
	}
	/*print_Cache(loc);*/
	free(term);
	return loc;
}
