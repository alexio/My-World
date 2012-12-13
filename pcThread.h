#ifndef PCTHREAD_H_
#define PCTHREAD_H_

#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

struct Customer {
	char* name;
	int id;
	double balance;
	char* address;
	struct Trans *Order_History;
	struct Trans* rejection;
	struct Customer * next;
};
typedef struct Customer *Customers;

struct Trans{
	char * book_title;
	double price;
	double cur_balance;
	struct Trans* next;
};
typedef struct Trans *History;

void init();

void loadCustomers(char * filename);

Customers initCustomer(char * name, double balance, int i, char * address);

void *producer(void *param);

void initTrans(History finder, Element order);

void handle_Order(Customers ptr, Element order);

void *consumer(void *param);

#endif
