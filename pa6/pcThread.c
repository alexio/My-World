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

void print(Customers customer) {
	Customers ptr;
	FILE *output = fopen("finalreport.txt", "w+");
	for (ptr = customer; ptr != NULL; ptr = ptr->next) {
		
		fprintf(output, "=== BEGIN CUSTOMER INFO ===\n");
		fprintf(output, "### BALANCE ###\n");
		fprintf(output, "Customer name: %s\n", ptr->name);
		fprintf(output, "Customer ID: %i\n", ptr->id);
		fprintf(output, "Remaining credit balance: %2f\n", ptr->balance);
		History success_ptr, failure_ptr;

		fprintf(output, "### SUCCESSFUL ORDERS ###\n");

		for (success_ptr = ptr->Order_History; 
			 success_ptr != NULL; 
			 success_ptr = success_ptr->next) {

			fprintf(output, "%s | %2f | %2f\n", 
				success_ptr->book_title, 
				success_ptr->price,
				success_ptr->cur_balance);
		}

		fprintf(output, "### REJECTED ORDERS ###\n");

		for (failure_ptr = ptr->rejection;
			 failure_ptr != NULL;
			 failure_ptr = failure_ptr->next) {

			fprintf(output, "%s | %2f\n", 
				failure_ptr->book_title, 
				failure_ptr->price);
		}
		fprintf(output, "=== END CUSTOMER INFO ===\n");
	}
	fclose(output);
	return;
}

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
	
	Customers ptr = database;
	while(ptr != NULL)
	{
		printf("Name: %s  Balance: %2f\n", ptr->name, ptr->balance);
		ptr = ptr->next;
	}
	
	print(database);
	return true;
}

/*producer thread reads in book orders from file creating queue
and will also read customer data*/
void * producer(void *param){
	
	char ** args = (char **)param;
	FILE * book_orders;
	if ((book_orders = fopen(args[2], "r")) == NULL) {
		fprintf(stderr, "./The book order file \"%s\" does not exist!\n", args[2]);
		return NULL;
	}
	/*creates queue*/
	pthread_mutex_lock(&mutex);
	loadCustomers(args[1]);
	orders = create_queue();
	append_books(orders,book_orders);
	
	/*creates database*/
	loadCustomers(args[1]);
	pthread_mutex_unlock(&mutex);
	
	Element qptr = orders->next_elem;
	while(qptr != NULL)
	{
		pthread_create(&tid, NULL, consumer, qptr);
		qptr = qptr->next_elem;
	}
		/*free(temp->book_title);
		free(temp->category);
		free(temp);*/
	return NULL;
}

History initTrans(Element order){
	
	History finder;
	if((finder = calloc(1, sizeof(struct Trans))) == NULL)
	{
		printf("Calloc failure\n");
		return NULL;
	}
	if((finder->book_title = calloc(strlen(order->book_title)+1, sizeof(char))) == NULL)
	{
		printf("Calloc failure\n");				
		return NULL;
	}
	strcpy(finder->book_title, order->book_title);
	finder->price = order->price;
	finder->cur_balance = 0;
	finder->next = NULL;
	return finder;
}
		
History handle_Order(Customers ptr, Element order){
	History finder;
	if(ptr->balance < order->price){ /*failed*/
		finder = ptr->rejection;
		if(finder == NULL){		
			finder = initTrans(order);
		}else{
			while(finder != NULL)
			{
				finder = finder->next;
			}	
			finder = initTrans(order);
		}
	}
	else{/*Successful Transaction*/
		finder = ptr->Order_History;
		if(finder == NULL){		
			finder = initTrans(order);
		}else{
			while(finder != NULL)
			{
				finder = finder->next;
			}	
			finder = initTrans(order);
		}
		ptr->balance -= order->price;
		finder->cur_balance = ptr->balance;
		/*printf("Book-Title: %s Customer: %s Balance: %2f\n",
		finder->book_title,ptr->name, finder->cur_balance);*/
	}
	return finder;
}
/*consumer handles an individual book order 
and extracting them one at a time*/
void *consumer(void *param){
	pthread_mutex_lock(&mutex);
	
	Element order = (Element)param;
	Customers ptr = database;
	int check = 0;
	while(ptr != NULL){
		
		if(ptr->id == order->id){
			check = 1;
			break;
		}
		else{
			ptr = ptr->next;
		}
	}
	
	if(check == 1){
		History finder;
		if(ptr->balance < order->price){ /*failed*/
			if (ptr->rejection == NULL) {
				ptr->rejection = handle_Order(ptr, order);
			} 
			else{
				finder = ptr->rejection;
				while(finder->next != NULL)
				{
					finder = finder->next;
				}	
				finder->next = handle_Order(ptr, order);
			}
		}
		else{/*Successful Transaction*/
			if (ptr->Order_History == NULL) {
				ptr->Order_History = handle_Order(ptr, order);
			} 
			else{
				finder = ptr->Order_History;
				while(finder->next != NULL)
				{
					finder = finder->next;
				}	
				finder->next = handle_Order(ptr, order);
			}
		}
	}
	else{
		printf("Customer not found\n");
		pthread_mutex_unlock(&mutex);
		return NULL;
	}
	pthread_mutex_unlock(&mutex);
	return NULL;
}



