#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cache.h"

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
	free (unit);
	return bytes;
}
