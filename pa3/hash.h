#ifndef HASH_H_
#define HASH_H_

struct docNode
{
	char * file_name;
	int frequency;
	struct docNode * next;
};
typdef struct docNode * docNode;

struct hashNode 
{
	char * term;
	docNode files_with_term;
	struct hashNode * next;
};
typedef struct hashNode * hashNode;

struct hashTable
{
	struct hashNode ** Htable; //unnecessary
	int size;
};
typedef struct hashTable * hashTable;

/*comments in hash.c*/
hashTable create_HashTable(int size);

int hash_Function(int size, char * string);

int insert_Hash(hashTable table, char * input);

int delete_from_Hash(hashTable table, char * input); /* not implemented*/

char * search_Hash(hashTable table, char * input); /* not implemented*/
