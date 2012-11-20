#ifndef SEARCHER_H_
#define SEARCHER_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include "tokenizer.h"
#include "hash.h"

struct File_Node
{
	int * index;
	struct File_Node* next;
};
typedef struct File_Node *File_Node;

int Hash_filescan(char *filename, char **files, hashTable tbl);
char **buildFileList(TokenizerT tokenizer, int file_nums);
hashTable buildHash(TokenizerT tokenizer, int term_num, int file_nums);
int *Search_And(int file_count, hashTable tbl, TokenizerT tokenizer);
int *Search_Or(int file_count, hashTable tbl, TokenizerT tokenizer);


#endif