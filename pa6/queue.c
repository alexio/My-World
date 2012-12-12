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
Queue create_queue(char *category) {
	Queue new_queue = calloc(3, sizeof(struct queue));
	new_queue->category = calloc(strlen(category), sizeof(char));
	strcpy(new_queue->category, category);
	new_queue->next_elem = NULL;
	new_queue->next_queue = NULL;
	return new_queue;
}

/*
 * Attempts to create the Element object with the given book title, price, and id
 */
Element create_elem(char *book_title, double price, int id) {
	Element new_element = calloc(4, sizeof(struct element));
	new_element->book_title = calloc(strlen(book_title), sizeof(char));
	strcpy(new_element->book_title, book_title);
	new_element->price = price;
	new_element->id = id;
	new_element->next_elem = NULL;
	return new_element;
}

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

Queue append_category(FILE *category_names) {
	Queue queue = NULL, ptr;
	char *stream;

	stream = calloc(minline, sizeof(char));
	while (fgets(stream, minline, category_names) != NULL) {
		printf("Length: %i", strlen(stream));
		stream[strlen(stream)] = '\0';
		if (queue == NULL) { /* if head queue is empty */
			queue = create_queue(stream);
			ptr = queue;
		} else { /* if head queue is not empty */
			if (queue->next_queue == NULL) { /* if next queue is empty */
				queue->next_queue = create_queue(stream);
			} else { /* loops to the next available queue */
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
