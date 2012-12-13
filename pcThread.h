#ifndef PCTHREAD_H_
#define PCTHREAD_H_

#include <stdio.h>
#include <stdlib.h>

struct Customer {
	char* name;
	int id;
	double balance;
	char* address;
	struct Customer * next;
};
typedef struct Customer *Customers;

Customers loadCustomers(char * filename);

Customers initCustomer(char * name, double balance, int i, char * address);

#endif
