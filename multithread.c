#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

/* this sets the max price amount */
#define max 100

/* file containing book names*/
FILE * books;

/* Creates the thread argument for producers */
struct data_t
{
	char *book_title;
	double price;
	int customer_id;
	pthread_mutex_t mutex;
	pthread_cond_t notEmpty;
	pthread_cond_t notFull;
};
typedef struct data_t *data;

/* Randomly generates a price */
double price_generator()
{
	double divide = RAND_MAX / max;
	double random = (rand()/divide);
	/* rounds to 2 decimal places */
	double rounded = floor(random * 100)/100;
	return rounded;
}

/* Generates the orders */
void* producer(void *arg)
{
	char line[max];
	data t_data = (data)arg;
	pthread_mutex_lock(&t_data->mutex);
  	fgets(line, max, books);
	if((t_data->book_title = (char *)calloc(strlen(line), sizeof(char))) == NULL)
	{
		fprintf(stderr, "malloc() failed in initialize");
		return NULL;
	}
	strcpy(t_data->book_title, line);
	t_data->price = price_generator();
	pthread_mutex_unlock(&t_data->mutex);
	pthread_exit(NULL);
}

data initialize()
{
	fprintf(stdout, "in initializer\n");
	data producer;
	/* Allocates memory for the struct thread_data_t */
	if ((producer = (data)malloc(sizeof(struct data_t))) == NULL) 
	{
		fprintf(stderr, "malloc() failed in initialize");
		return NULL;
	}
	pthread_mutex_init(&producer->mutex, 0);
	pthread_cond_init(&producer->notFull, 0);
	pthread_cond_init(&producer->notEmpty, 0);
	return producer;
}

int main(int argc, char **argv)
{
	int i, check;
	data t_data;
	pthread_t thread[atoi(argv[1])-1];
	if (argc != 3) 
	{
		fprintf(stderr, "./multithread <number of orders> <mean interval>\n");
		return 1;
	}
	
	/* Opens the dict.txt file */
	books = fopen("dict.txt", "r");
	if (books == NULL)
	{
		printf("The \"dict.txt\" file is missing.\n");
		return 0;
	}

	/* create threads */
	
	for (i = 0; i < atoi(argv[1]) ; i++)
	{	
		t_data = initialize();
		check = pthread_create(&thread[i], NULL, producer, t_data);
		if (check != 0)
		{
			fprintf(stderr, "pthread_create error #%d\n", check);
			free(t_data);
			return false;
		}
		fprintf(stdout, "%lu\n", thread[i]);
	}
	
	/* Waits until all the threads are completed */
	for (i = 0; i < atoi(argv[1]); i++)
	{
		pthread_join(thread[i], NULL);
	}

	return true;
}
