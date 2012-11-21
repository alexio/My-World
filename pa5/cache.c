#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "cache.h"
#include "hash.h"

/*
 * Attempts to take the substring of a given string,
 *  from the given start to the given end.
 */
char *substring(int start, int end, char *word) {
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
	unsigned long int bytes = atoi(substring(0, length-2, limit));
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

hashTable filter(FILE *fileptr, int term_num) {
	char *term, *token;
	int *bytes, size = 30;
	fseek(fileptr, 0L, SEEK_SET);
	printf("# terms: %i\n", term_num);
	hashTable loc = create_HashTable(term_num);

	if ((term = calloc(size, sizeof(char))) == NULL) {
		printf("Not enough memory in cache.c @ line 51\n");
		return NULL;
	}

	while (fgets(term, 7, fileptr) != NULL) {
		/*printf("<%i> %s\n", count, term);*/
		if (strcmp(term, "<list>") == 0) {
			if ((bytes = (int *)calloc(1, sizeof(int))) == NULL ||
				(token = calloc(size, sizeof(char))) == NULL) {
				printf("Not enough memory in cache.c @ line 59\n");
				return NULL;
			}
			memset(bytes, 0, 1);
			fgets(token, size, fileptr);
			int length = strlen(token);
			/* Trims off the "\n" */
			token[length-1] = '\0';
			bytes[0] = ftell(fileptr);
			/*printf("<%i> %s\n", bytes[1], token);*/
			/*insert_Hash(loc, token, bytes, 1);*/
			free(bytes);
			free(token);
		}
		free(term);
		if ((term = calloc(7, sizeof(char))) == NULL) {
			printf("Not enough memory in cache.c @ line 61\n");
			return NULL;
		}
	}
	print_Hash(loc);
	free(bytes);
	free(term);
	return loc;
}
