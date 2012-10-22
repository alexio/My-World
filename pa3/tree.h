#ifndef INDEX_H_
#define INDEX_H_

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>

struct filepointer {
	char * filename;
	int frequency;
	struct filepointer * next;
};
typedef struct filepointer * fpointer;


struct tree {
	char * token;
	fpointer files;
	struct tree * left;
	struct tree * right;
};
typedef struct tree * Tree;

Tree treeCreate();

fpointer fpointerCreate();

void increase(fpointer head, char * file);

void sort_byFrequency(fpointer point);

void print(Tree root, FILE * pointer);

void insert_Tree(Tree root, char * term, char * file);

void freelist(fpointer head);

void freeup(Tree root);
#endif
