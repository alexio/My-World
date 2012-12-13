#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "pcThread.h"
#include "queue.h"
#include "tokenizer.h"

void *producer(void *param);
void *consumer(void *param);
void init();

pthread_t tid;
pthread_attr_t attr; /*thread attrributes*/

/*Mutex lock*/
pthread_mutex_t mutex;

Queue orders;
	
void init()
{
	pthread_mutex_init(&mutex, NULL);
	pthread_attr_init(&attr);
}

Customers initCustomer(char * name, double balance, int id, char * address){
	
	if(name == NULL || address == NULL){
		printf("Input value null, exit\n");
		return NULL;
	}	 
	
	Customers ptr;
	if((ptr = calloc(1, sizeof(struct Customer))) == NULL){
		printf("Database calloc failed\n");
		return NULL;
	}
	if((ptr->name = calloc(strlen(name)+1, sizeof(char))) == NULL){
		printf("Database Name calloc failed\n");
		return NULL; 
	}
	
	if((ptr->address = calloc(strlen(address)+1, sizeof(char))) == NULL){
		printf("Database Address calloc failed\n");
		return NULL;
	}
	
	strcpy(ptr->name, name);
	strcpy(ptr->address, address);
	ptr->id = id;
	ptr->balance = balance;
	return ptr;
}

Customers loadCustomers(char * filename){
	
	FILE *database_file;
	if ((database_file = fopen(filename, "r")) == NULL) {
		fprintf(stderr, "./The database file \"%s\" does not exist!\n", filename);
		return false;
	}
	
	Customers database;
	if((database = calloc(1, sizeof(struct Customer))) == NULL){
		printf("Database calloc failed\n");
		return NULL;
	}
	
	char *stream, *name, *line, *endptr;
	double balance;
	int id;
	
	TokenizerT tokenizer;
	int length;
	
	char * address = calloc(500, sizeof(char));
	stream = calloc(500, sizeof(char));
	Customers ptr = database;
	while (fgets(stream, 500, database_file) != NULL) {
		length = strlen(stream);
		stream[length-1] = '\0';
		
		tokenizer = TKCreate("\"|", stream);
		name = TKGetNextToken(tokenizer);
		id = strtol(TKGetNextToken(tokenizer), &endptr, 0);
		balance = strtod(TKGetNextToken(tokenizer), &endptr);
		
		/*puts address into one char* */
		line = TKGetNextToken(tokenizer);
		while(line != NULL){
			strcat(address, line);
			free(line);
			line = TKGetNextToken(tokenizer);
		}
		/* end of address creation */
		
		ptr = initCustomer(name, balance, id, address);
		printf("CHeck: %s\n", ptr->name);
		ptr = ptr->next;
	
		TKDestroy(tokenizer);
		free(stream);
		free(name);
		free(address);
		stream = calloc(500, sizeof(char));
		address = calloc(500, sizeof(char));
	}
	free(stream);
	free(address);
	Customers ptr2 = database;
	printf("Sup\n");
	while(ptr2 != NULL){
		printf("Name: %s\n", ptr2->name);
		ptr2 = ptr2->next;
	}	
	printf("Nothing\n");
	return NULL;
}

int main(int argc, char **argv) {

	/*FILE *database_file, *book_orders, *category_names;*/

	if (argc != 4) {
		fprintf(stderr, "./multithread <name of database> <name of book order> <list of category names>\n");
		return false;
	}
	/*if ((category_names = fopen(argv[3], "r")) == NULL) {
		fprintf(stderr, "./The category file \"%s\" does not exist!\n", argv[3]);
		return false;
	}*/

	/*init();
	
	pthread_t pth;
	printf("Going\n");
	
	pthread_create(&pth, NULL, producer, argv);
	pthread_join(pth, NULL);*/
	
	loadCustomers(argv[1]);
	return true;
}

/*producer thread reads in book orders from file creating queue
and will also read customer data*/
void * producer(void *param){
	
	printf("Here\n");
	char ** args = (char **)param;
	printf("Files: %s\n", args[2]);
	FILE * book_orders;
	if ((book_orders = fopen(args[2], "r")) == NULL) {
		fprintf(stderr, "./The book order file \"%s\" does not exist!\n", args[2]);
		return NULL;
	}
	
	pthread_mutex_lock(&mutex);
	loadCustomers(args[1]);
	orders = create_queue();
	append_books(orders,book_orders);
	
	printf("Here they are\n");
	eread(orders->next_elem);
	pthread_mutex_unlock(&mutex);
	
	while(1)
	{
		pthread_create(&tid, NULL, consumer, orders);
	}
	return NULL;
}


/*consumer handles an individual book order 
and extracting them one at a time*/
void *consumer(void *param){
	pthread_mutex_lock(&mutex);
	
	while(1)
	{
		
		
		
	}
	pthread_mutex_unlock(&mutex);
	return NULL;
}



