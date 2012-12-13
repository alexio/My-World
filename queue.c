#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"
#include "tokenizer.h"

#define maxline 256
#define minline 100

/*
 * Attempts to create the Queue object with the given category name
 */
Queue create_queue() {
	Queue new_queue = calloc(1, sizeof(struct queue));
	new_queue->next_elem = NULL;
	return new_queue;
}

/*
 * Attempts to create the Element object with the given book title, price, and id
 */
Element create_elem(char *book_title, double price, int id, char *category) {
	Element new_element = calloc(5, sizeof(struct element));
	new_element->book_title = calloc(strlen(book_title), sizeof(char));
	strcpy(new_element->book_title, book_title);
	new_element->price = price;
	new_element->id = id;
	new_element->category = calloc(strlen(category), sizeof(char));
	strcpy(new_element->category, category);
	new_element->next_elem = NULL;
	return new_element;
}

/*
 * Helper method to qread(),
 *  will attempt to printf() the element object
 */
void eread(Element elem) {
	Element ptr;
	if (elem != NULL) {
		for (ptr = elem; ptr->next_elem != NULL; ptr = ptr->next_elem) {
			printf("[%s]\n $%.2f : %i : %s\n", ptr->book_title, ptr->price, ptr->id, ptr->category);
		}
		printf("[%s] $%f : %i : %s\n", ptr->book_title, ptr->price, ptr->id, ptr->category);
	} else {
		printf("No element!\n");
	}
	return;
}

void enqueue(Queue queue, char *book_title, double price, int id, char *category) {
	Element elem, ptr;
	elem = create_elem(book_title, price, id, category);
	if (queue->next_elem == NULL) {
		queue->next_elem = elem;
	} else {
		ptr = queue->next_elem;
		while (ptr->next_elem != NULL) {
			ptr = ptr->next_elem;
		}
		ptr->next_elem = elem;
	}
	return;
}

/*
 * Attempts to printf() the queue object
 *
void qread(Queue queue) {
	Queue ptr;
	if (queue != NULL) {
		for (ptr = queue; ptr->next_queue != NULL; ptr = ptr->next_queue) {
			printf("Category: \"%s\"\n", ptr->category);
		}
		printf("Category: \"%s\"\n", ptr->category);
	} else {
		printf("Queue is empty!\n");
	}
	return;
}
*/

/*
Queue append_categories(FILE *category_names) {
	Queue queue = NULL, ptr;
	char *stream;
	int length;

	stream = calloc(minline, sizeof(char));
	while (fgets(stream, minline, category_names) != NULL) {
		length = strlen(stream);
		stream[length-1] = '\0';
		if (queue == NULL) {
			queue = create_queue(stream);
			ptr = queue;
		} else {
			if (queue->next_queue == NULL) {
				queue->next_queue = create_queue(stream);
			} else {
				while (ptr->next_queue != NULL) {
					ptr = ptr->next_queue;
				}
				ptr->next_queue = create_queue(stream);
			}
		}
		free(stream);
		stream = calloc(minline, sizeof(char));
	}
	return queue;
}
*/

Queue append_books(Queue queue, FILE *book_orders) {
	char *stream, *book_title, *category, *endptr;
	double price;
	int id;
	TokenizerT tokenizer;
	int length;

	stream = calloc(maxline, sizeof(char));
	while (fgets(stream, maxline, book_orders) != NULL) {
		length = strlen(stream);
		stream[length-1] = '\0';

		tokenizer = TKCreate("\"|", stream);
		book_title = TKGetNextToken(tokenizer);
		price = strtod(TKGetNextToken(tokenizer), &endptr);
		id = strtol(TKGetNextToken(tokenizer), &endptr, 0);
		category = TKGetNextToken(tokenizer);

		enqueue(queue, book_title, price, id, category);

		TKDestroy(tokenizer);
		free(stream);
		stream = calloc(maxline, sizeof(char));
	}
	return queue;
}
