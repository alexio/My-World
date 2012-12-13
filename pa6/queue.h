#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdio.h>
#include <stdlib.h>

struct element {
	char *book_title;
	double price;
	int id;
	char *category;
	struct element *next_elem;
}; 
typedef struct element *Element;

struct queue {
	struct element *next_elem;
}; 
typedef struct queue *Queue;

Queue create_queue();

Element create_elem(char *book_title, double price, int id, char *category);

void eread(Element elem);

void enqueue(Queue queue, char *book_title, double price, int id, char *category);

Queue append_books(Queue queues, FILE *book_orders);

#endif
