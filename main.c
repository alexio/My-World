#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>


#include "pcThread.h"
#include "queue.h"
#include "tokenizer.h"

pthread_t tid; /*thread id*/
pthread_attr_t attr; /*thread attrributes*/
buffer_item buffer[BUFFER_SIZE];

/*Semaphore and Mutex lock*/
sem_t cEmpty;
sem_t cFull;
pthread_mutex_t mutex;

int cg; /*progress counter*/
	
void *get_data(void * arg){
	char *str;
	int i = 0;

	str=(char*)arg;

	while(i < 5 )
	{
		usleep(1);
		printf("threadFunc says: %s\n",str);
		++i;
	}

	return NULL;
}

void init()
{
	int c2;
	pthread_mutex_init(&mutex, NULL);
	pthread_attr_init(&attr);
	sem_init(&cFull, 0, 0);
	sem_init(&cEmpty, 0, BUFFER_SIZE);
	cg = 0;
	for(c2=0; c2<BUFFER_SIZE; c2++){
		buffer[c2] =0;
	}
}

int main(int argc, char **argv) {

	FILE *database_file, *book_orders, *category_names;
	Queue queue;

	if (argc != 4) {
		fprintf(stderr, "./multithread <name of database> <name of book order> <list of category names>\n");
		return false;
	}
	/*if ((database_file = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "./The database file \"%s\" does not exist!\n", argv[1]);
		return false;
	}*/
	if ((book_orders = fopen(argv[2], "r")) == NULL) {
		fprintf(stderr, "./The book order file \"%s\" does not exist!\n", argv[2]);
		return false;
	}
	/*if ((category_names = fopen(argv[3], "r")) == NULL) {
		fprintf(stderr, "./The category file \"%s\" does not exist!\n", argv[3]);
		return false;
	}*/

	int();
	
	/*producer thread*/
	pthread_create(&tid, &attr, producer, book_orders);
	
	
	/*queue = create_queue();
	append_books(queue, book_orders);
	eread(queue->next_elem);*/
	return true;
}
