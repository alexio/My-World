
#include "hash.h"
#include <stdlib.h>
#include <stdio.h>

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
				/*
				 *
				 *
				if(docPtr->doc != NULL)
				{
					free(docPtr->doc);
				}
				 *
				 *
				 */
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

/*adds file where term is exists or increments frequency*/
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
	}
	
	if(strcmp(term_node->files_with_term->file_name, doc) == 0) /*same as head*/
	{
		term_node->files_with_term->frequency++;
		free(new);
		return 1;
	}
	
	if(strcmp(term_node->files_with_term->file_name, doc) < 0) /*item goes before head*/
	{
		new->next = term_node;
		term_node = new;
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
	
	int index = hash_Function(table->size, input);
	
	hashNode new;
	if((new = calloc(1, sizeof(struct hashNode))) == 0)
	{
		return NULL;
	}
	new->term = input; 
	
	if(table->Htable[index] == NULL) /*list is empty*/
	{
		table->Htable[index] = new;
		/*
		 *
		 *
		insert_Doc(table->Htable[index], index, doc);
		 *
		 *
		 */
		insert_Doc(table->Htable[index], doc);
	}
	
	if(strcmp(table->Htable[index]->term, input) == 0)/*same as head*/
	{
		insert_Doc(table->Htable[index], doc);
		return 1;
	}
	
	if(strcmp(table->Htable[index]->term, input) < 0)/*goes before head*/
	{
		new->next = table->Htable[index];
		table->Htable[index] = new;
		insert_Doc(table->Htable[index], doc); /* could just point it?*/
		return 1;
	}
	
	hashNode ptr = table->Htable[index];
	while(ptr->next != NULL)
	{
		if(strcmp(ptr->next->term, input) == 0)/*same as current node*/
		{
			insert_Doc(ptr->next, doc);
			return 1;
		}
		else if(strcmp(ptr->next->term, input) < 0) /*goes before current*/
		{
			new->next = ptr->next;
			ptr->next = new;
			insert_Doc(ptr->next, doc);
			return 1;
		}
		ptr = ptr->next;
	}
	
	ptr->next = new; /*goes at the end*/
	insert_Doc(ptr->next, doc);
	return 1;
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
