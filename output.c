#include <stdio.h>
#include <stdlib.h>



void print_helper(History Order_History, History rejection) {
	
	FILE output = fopen(categories.txt, "a+");

	History success_ptr, failure_ptr;

	printf("### SUCCESSFUL ORDERS ###\n");

	for (success_ptr = Order_History; 
		 success_ptr != NULL; 
		 success_ptr = success_ptr->next) {

		fprintf(output, "%s | %2f | %2f\n", 
			success_ptr->book_title, 
			success_ptr->price,
			success_ptr->cur_balance);
	}

	printf("### REJECTED ORDERS ###\n");

	for (failure_ptr = Order_History;
		 failure_ptr != NULL;
		 failure_ptr = failure_ptr) {

		printf(output, "%s | %2f | %2f\n", 
			success_ptr->book_title, 
			success_ptr->price;
	}
	return;
}

void print(Customers customer) {
	Customers ptr;

	printf("=== BEGIN CUSTOMER INFO ===\n");
	printf("### BALANCE ###\n");
	printf("Customer name: %s\n", customer->name);
	printf("Customer ID: %i\n", customer->id);
	printf("Remaining credit balance: %d\n", customer->balance);

	for (ptr = customer; ptr != NULL; ptr = ptr->next) {
		print_helper(ptr->Order_History, ptr->rejection);
	}
	
	printf("=== END CUSTOMER INFO ===\n");

	return;
}