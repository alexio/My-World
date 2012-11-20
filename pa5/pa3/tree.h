#ifndef INDEX_H_
#define INDEX_H_

#include <stdio.h>
#include <string.h>
#include <ctype.h>

struct filepointer {
	char * filename;
	int frequency;
	struct filepointer *next;
};
typedef struct filepointer *fpointer;

struct tree {
	char *token;
	fpointer files;
	struct tree *left;
	struct tree *right;
};
typedef struct tree *Tree;

struct counter {
	int word_count;
	int file_count;
};
typedef struct counter *Counter;

struct filename {
	char *name;
	struct filename *next;
};
typedef struct filename *Files;

Tree treeCreate();

fpointer fpointerCreate();

void increase(fpointer head, char *file);

void sort_byFrequency(fpointer point);

int search(Files filenames, char *file);

void print(Tree root, FILE *pointer, Files filenames);

void print_file(FILE *ptr, Files filenames);

void insert_Tree(Tree root, char *term, char *file, Counter counter);

void freelist(fpointer head);

void freeup(Tree root);

void free_filenames(Files filenames);
#endif
