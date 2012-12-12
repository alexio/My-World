#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "queue.h"
#include "tokenizer.h"

int main(int argc, char **argv) {

	FILE *database_file, *book_order, *category_names;
	Queue queues;

	if (argc != 4) {
		fprintf(stderr, "./multithread <name of database> <name of book order> <list of category names>\n");
		return false;
	}
	/*if ((database_file = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "./The database file \"%s\" does not exist!\n", argv[1]);
		return false;
	} 
	if ((book_order = fopen(argv[2], "r")) == NULL) {
		fprintf(stderr, "./The book order file \"%s\" does not exist!\n", argv[2]);
		return false;
	}*/
	if ((category_names = fopen(argv[3], "r")) == NULL) {
		fprintf(stderr, "./The category file \"%s\" does not exist!\n", argv[3]);
		return false;
	}

	queues = append_category(category_names);
	fclose(category_names);
	qread(queues);

	return true;
}