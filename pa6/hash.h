#ifndef HASH_H_
#define HASH_H_

#include <stdlib.h>
#include <stdio.h>

struct hashNode 
{
	char * term;
	int * files;
	struct hashNode * next;
};
typedef struct hashNode * hashNode;

struct hashTable
{
	struct hashNode ** Htable; /*unnecessary*/
	int size;
};
typedef struct hashTable * hashTable;

/*comments in hash.c*/
hashTable create_HashTable(int size);

void destroy_HashTable(hashTable table);

int hash_Function(int size, char * string);

int insert_Hash(hashTable table, char * input, int * file_array, int file_nums);

void print_Hash(hashTable tbl);

int delete_from_Hash(hashTable table, int file_nums, int memsize);

int * search_Hash(hashTable table, char * input);

#endif