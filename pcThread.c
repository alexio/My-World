#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "pcThread.h"
#include "queue.h"
#include "tokenizer.h"

pthread_t tid;
pthread_attr_t attr; /*thread attrributes*/

/*Mutex lock*/
pthread_mutex_t mutex;

Queue orders;
Customers database;
	
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

	ptr->Order_History = NULL;
	ptr->rejection = NULL;
	strcpy(ptr->name, name);
	strcpy(ptr->address, address);
	ptr->id = id;
	ptr->balance = balance;
	return ptr;
}

void loadCustomers(char * filename){
	
	FILE *database_file;
	if ((database_file = fopen(filename, "r")) == NULL) {
		fprintf(stderr, "./The database file \"%s\" does not exist!\n", filename);
		return;
	}
	
	database = NULL;
	
	char *stream, *name, *line, *endptr;
	double balance;
	int id;
	
	TokenizerT tokenizer;
	int length;
	
	char * address = calloc(500, sizeof(char));
	stream = calloc(500, sizeof(char));
	Customers ptr;
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
		
		if(database == NULL){
			database = initCustomer(name, balance, id, address);
			ptr = database;
		}
		else{
			ptr->next = initCustomer(name, balance, id, address);
			ptr = ptr->next;
		}
	
		TKDestroy(tokenizer);
		free(stream);
		free(name);
		free(address);
		stream = calloc(500, sizeof(char));
		address = calloc(500, sizeof(char));
	}
	free(stream);
	free(address);
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

	init();
	
	pthread_t pth;
	pthread_create(&pth, NULL, producer, argv);
	pthread_join(pth, NULL);
	
	
	return true;
}

/*producer thread reads in book orders from file creating queue
and will also read customer data*/
void * producer(void *param){
	
	char ** args = (char **)param;
	printf("Files: %s\n", args[2]);
	FILE * book_orders;
	if ((book_orders = fopen(args[2], "r")) == NULL) {
		fprintf(stderr, "./The book order file \"%s\" does not exist!\n", args[2]);
		return NULL;
	}
	/*creates queue*/
	/*pthread_mutex_lock(&mutex);*/
	loadCustomers(args[1]);
	orders = create_queue();
	append_books(orders,book_orders);
	
	/*creates database*/
	loadCustomers(args[1]);
	/*pthread_mutex_unlock(&mutex);*/
	
	Element qptr = orders->next_elem;
	while(qptr != NULL)
	{
		/*pthread_create(&tid, NULL, consumer, qptr);*/
		consumer(qptr);
		qptr = qptr->next_elem;
	}
		/*free(temp->book_title);
		free(temp->category);
		free(temp);*/
	return NULL;
}

void initTrans(History finder, Element order){
	printf("Trans\n");
	if((finder->book_title = calloc(strlen(order->book_title)+1, sizeof(char))) == 0)
	{
		printf("Calloc failure\n");				
		return;
	}
	printf("Mid of Init: %s\n", order->book_title);
	strcpy(finder->book_title, order->book_title);
	finder->price = order->price;
	finder->next = NULL;
	printf("End of Init\n");
}

void handle_Order(Customers ptr, Element order){
	printf("Here\n");
	History finder;
	if(ptr->balance < order->price){ /*failed*/
		printf("Failed\n");
		finder = ptr->rejection;
		if(finder == NULL){		
			initTrans(finder, order);
		}else{
			while(finder != NULL)
			{
				finder = finder->next;
			}	
			initTrans(finder, order);
		}
	}
	else{/*Successful Transaction*/
		printf("Good\n");
		finder = ptr->Order_History;
		if(finder == NULL){	
			printf("Finder NULL\n");	
			initTrans(finder, order);
		}else{
			printf("Finder NOT NULL\n");
			while(finder != NULL)
			{
				finder = finder->next;
			}	
			initTrans(finder, order);
		}
		ptr->balance -= order->price;
		finder->cur_balance = ptr->balance;
	}
}
/*consumer handles an individual book order 
and extracting them one at a time*/
void *consumer(void *param){
	printf("here\n");
	/*pthread_mutex_lock(&mutex);*/
	
	Element order = (Element)param;
	Customers ptr = database;
	int check = 0;
	printf("NameSUP: %s WHY %d %2f\n", order->book_title, order->id, order->price);
	printf("Before Loop: ID -> %d Order: %d\n", ptr->id, order->id);
	if(order->price == 0){
		printf("FUCK YOU\n");
	}
	while(ptr != NULL){
		
		if(ptr->id == order->id){
			check = 1;
			break;
		}
		else{
			printf("ID -> %d\n", ptr->id);
			ptr = ptr->next;
		}
	}
	
	if(check == 1){
		printf("Going in\n");
		handle_Order(ptr, order);
	}
	else{
		printf("Customer not found\n");
		pthread_mutex_unlock(&mutex);
		return NULL;
	}
	/*pthread_mutex_unlock(&mutex);*/
	return NULL;
}



