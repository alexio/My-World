
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
	hashNode hashPtr;
	for(a = 0; a < table->size; a++)
	{
		hashPtr = table->Htable[a];
		while(hashPtr != NULL)
		{
			if(hashPtr->term != NULL)
			{
				free(hashPtr->term);
			}
			
			hashNode Temp = hashPtr;
			int * filetemp = Temp->files;
			free(filetemp);
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

/*insert item into hash
returns 1 if successful and 0 is unsuccessful*/
int insert_Hash(hashTable table, char * input, int * file_array, int file_nums)
{
	
	if(input == NULL) /*bad input*/
	{
		return 0;
	}
	
	/*for(i = 0; input[i]; i++){
	  input[i] = tolower(input[i]);
	}*/
	
	int i, index = hash_Function(table->size, input);

	int * files;
	if ((files = (int *)calloc(file_nums, sizeof(int))) == NULL)
	{
		printf("Not enough Memory\n");
		return 0;
	}

	memset(files, 0, file_nums);

	for(i = 0; i < file_nums; i++)
	{
		files[i] = file_array[i];
	}
	
	hashNode new;
	if((new = calloc(1, sizeof(struct hashNode))) == 0) {
		return 0;
	}
	new->term = calloc(strlen(input)+1, sizeof(char)); 
	strcpy(new->term, input);
	new->files = files;
	
	
	if(table->Htable[index] == NULL) /*list is empty*/
	{
		table->Htable[index] = new;
		return 1;
	}
	/*same as head*/
	if(strcmp(table->Htable[index]->term, input) == 0) {
		return 1;
	}
	/*goes before head*/
	if(strcmp(table->Htable[index]->term, input) > 0) {
		
		new->next = table->Htable[index];
		table->Htable[index] = new;
		return 1;
	}
	
	hashNode ptr = table->Htable[index];
	while(ptr->next != NULL) {
		if(strcmp(ptr->next->term, input) == 0) { /*same as current node*/
			
			return 1;
		}
		else if(strcmp(ptr->next->term, input) > 0) { /*goes before current*/
			
			new->next = ptr->next;
			ptr->next = new;
			return 1;
		}
		ptr = ptr->next;
	}
	/*goes at the end*/
	ptr->next = new;
	return 1;
}

void print_Hash(hashTable tbl)
{	
	int i;
	hashNode h_ptr = NULL;
	for(i = 0; i < tbl->size; i++)
	{
		h_ptr = tbl->Htable[i];
		while(h_ptr != NULL)
		{
			printf("Term %s\n", h_ptr->term);

			int j;
			for(j = 0; j < 12; j++)
			{
				printf("File: %d, Frequency %d\n", j, h_ptr->files[j]);
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
int * search_Hash(hashTable table, char * input)
{

	int * file_array = NULL;
	int hashval = hash_Function(table->size, input);
	hashNode ptr = table->Htable[hashval];
	while(ptr != NULL)
	{
		if(strcmp(input, ptr->term) == 0)/*same*/
		{
			file_array = ptr->files;
			return file_array;
		}
		ptr = ptr->next;
	}
	return NULL; /*term not found*/
}
