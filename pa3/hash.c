
#include "hash.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Initialize hash table*/
hashTable create_HashTable(int size)
{
	if(size < 1)
	{
		return NULL;
	}
	hashTable table;
	if((table = calloc(1, sizeof(struct hashTable))) == 0)
	{
		return NULL;
	}
	
	if((table->Htable = calloc(size, sizeof(struct hashNode))) == 0)
	{
		return NULL;
	}
	
	table->size = size;
	return table;
}

/*frees hashtable*/
void destroy_HashTable(hashTable table)
{

	if(table == NULL)
	{
		return;
	}
	int a;
	docNode docPtr;
	hashNode hashPtr;
	for(a = 0; a < table->size; a++)
	{
		hashPtr = table->Htable[a];
		while(hashPtr != NULL)
		{
			docPtr = hashPtr->files_with_term;
			while(docPtr!=NULL)
			{
				if(docPtr->next != NULL)
				{
					free(docPtr->next);
				}
				docNode temp = docPtr;
				docPtr = docPtr->next;
				free(temp);
			}
			
			if(hashPtr->term != NULL)
			{
				free(hashPtr->term);
			}
			
			hashNode Temp = hashPtr;
			hashPtr = hashPtr->next;
			free(Temp);
		}
		
	}
	
	free(table->Htable);
	free(table);
}


/*calculates the index for hash insertion, deletion, or search
returns index*/
int hash_Function(int size, char * string)
{
	int a, hashval = 0;
	
	for(a = 0; a < strlen(string); a++)
	{
		hashval+= string[a];
	}
	
	return hashval % size;
}

/*adds file where term already existed and increments the frequency*/
int insert_Doc(hashNode term_node, char * doc)
{
	docNode new;
	if((new = calloc(1, sizeof(struct docNode))) == 0)
	{
		return 0;
	}
	new->file_name = doc;
	new->frequency =1;
	
	if(term_node->files_with_term == NULL) /*file list is null*/
	{
		term_node->files_with_term = new;
		return 1;
	}
	
	if(strcmp(term_node->files_with_term->file_name, doc) == 0) /*same as head*/
	{
		term_node->files_with_term->frequency++;
		free(new);
		return 1;
	}
	
	if(strcmp(term_node->files_with_term->file_name, doc) < 0) /*item goes before head*/
	{
		new->next = term_node->files_with_term;
		term_node->files_with_term = new;
		return 1;
	}
	
	docNode ptr = term_node->files_with_term;
	while(ptr->next != NULL)
	{
		if(strcmp(ptr->next->file_name, doc) == 0) /*same as current node*/
		{
			ptr->next->frequency++;
			free(new);
			return 1;
		}
		else if(strcmp(ptr->next->file_name, doc) < 0)/*goes before current node*/
		{
			new->next = ptr->next;
			ptr->next = new;
			return 1;
		}
		ptr = ptr->next;
	}
	
	ptr->next = new; /*item goes at the end of the list*/
	return 1;
}

/*insert item into hash
returns 1 if successful and 0 is unsuccessful*/
int insert_Hash(hashTable table, char * input, char * doc)
{
	if(input == NULL || doc == NULL) /*bad input*/
	{
		return 0;
	}
	
	int i, index = hash_Function(table->size, input);

	for(i = 0; input[i]; i++){
	  input[i] = tolower(input[i]);
	}
	
	//printf("Input: %s , Doc: %s, Hash #: %d\n", input, doc, index);

	hashNode new;
	if((new = calloc(1, sizeof(struct hashNode))) == 0) {
		return 0;
	}
	new->term = calloc(strlen(input)+1, sizeof(char)); 
	strcpy(new->term, input);
	
	//printf("New Node: %s\n", new->term);
	
	if(table->Htable[index] == NULL) /*list is empty*/
	{
		//printf("Add New Head\n");
		table->Htable[index] = new;
		insert_Doc(table->Htable[index], doc);
		return 1;
	}
	/*same as head*/
	if(strcmp(table->Htable[index]->term, input) == 0) {
		//printf("same as head\n");
		insert_Doc(table->Htable[index], doc);
		return 1;
	}
	/*goes before head*/
	if(strcmp(table->Htable[index]->term, input) > 0) {
		//printf("Before Head\n");
		new->next = table->Htable[index];
		table->Htable[index] = new;
		insert_Doc(table->Htable[index], doc);
		return 1;
	}
	
	hashNode ptr = table->Htable[index];
	while(ptr->next != NULL) {
		if(strcmp(ptr->next->term, input) == 0) { /*same as current node*/
			//printf("Same in loop\n");
			insert_Doc(ptr->next, doc);
			return 1;
		}
		else if(strcmp(ptr->next->term, input) > 0) { /*goes before current*/
			//printf("Goes before current node");
			new->next = ptr->next;
			ptr->next = new;
			insert_Doc(ptr->next, doc);
			return 1;
		}
		ptr = ptr->next;
	}
	/*goes at the end*/
	ptr->next = new;
	insert_Doc(ptr->next, doc);
	return 1;
}

void print_Hash(hashTable tbl)
{	
	int i;
	hashNode h_ptr = NULL;
	docNode f_ptr = NULL;
	for(i = 0; i < tbl->size; i++)
	{
		h_ptr = tbl->Htable[i];
		while(h_ptr != NULL)
		{
			printf("Term %s\n", h_ptr->term);
			f_ptr = h_ptr->files_with_term;
			while(f_ptr != NULL)
			{
				printf("File: %s \n", 
					f_ptr->file_name);
				printf("Frequency %d \n", f_ptr->frequency);
				f_ptr = f_ptr->next;
			}
			h_ptr = h_ptr->next;
		}
	}
}


/*delete item from hash, returns 1 if successful and 0 is unsuccessful*/
int delete_from_Hash(hashTable table, char * input)
{
	/*not implemented*/
	return 0;
}

/*search has for an item*/
char* search_Hash(hashTable table, char * input)
{
	/*not implemented*/
	return NULL;
}
