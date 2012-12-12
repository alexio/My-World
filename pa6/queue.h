#ifndef QUERY_H_
#define QUERY_H_

#include <stdio.h>
#include <stdlib.h>

struct element {
	char *book_title;
	double price;
	int id;
	char *category;
	struct element *next_elem;
}; typedef struct element *Element;

struct queue {
	struct element *next_elem;
}; typedef struct queue *Queue;

Queue create_queue();

Element create_elem(char *book_title, double price, int id, char *category);

void eread(Element elem);

void enqueue(Queue queue, book_title, price, id, category);
/*
void qread(Queue queue);

Queue append_categories(FILE *category_names);
*/
Queue append_books(queues, book_orders);

#endif
