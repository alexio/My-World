#ifndef QUERY_H_
#define QUERY_H_

#include <stdio.h>
#include <stdlib.h>

struct element {
	char *book_title;
	double price;
	int id;
	struct element *next_elem;
}; typedef struct element *Element;

struct queue {
	char *category;
	struct element *next_elem;
	struct queue *next_queue;
}; typedef struct queue *Queue;

Queue create_queue(char *category);

Element create_elem(char *book_title, double price, int id);

void qread(Queue queue);

Queue append_category(FILE *category_names);

#endif