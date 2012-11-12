
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include "tree.h"

Tree treeCreate()
{
	
	Tree ans;
	if ((ans = calloc(1, sizeof(struct tree))) == 0) {
		exit(1);
	}
	ans->token = NULL;
	ans->files = NULL;
	ans->left = NULL;
	ans->right = NULL;
	return ans;
}

fpointer fpointerCreate() {
	
	fpointer ans;
	if ((ans = calloc(1, sizeof(struct filepointer))) == 0) {
		exit(1);
	}
	ans->filename = NULL;
	ans->frequency = 0;
	ans->next = NULL;
	return ans;
}

/*adds file to file list or increases frequency*/
void increase(fpointer head, char * file) {

	if(file == NULL)
	{
		printf("File name is null");
		return;
	}
	fpointer ptr = head;
	fpointer temp;
	while (1) {
		
		if (strcasecmp(ptr->filename, file) == 0) {
			ptr->frequency++;
			return;
		}
		if (ptr->next != NULL) {
			ptr = ptr->next;
		}
		else {
			temp = fpointerCreate();
			temp->filename = calloc(strlen(file)+1, sizeof(char)); 
			strcpy(temp->filename, file);
			temp->frequency++;
			ptr->next = temp;
			return;
		}
	}
}

/*sorts files in which a term appears by frequency*/
void sort_byFrequency(fpointer point){
	
	if(point == NULL || point->next == NULL){
		
			return;
	}

	fpointer ptr = point;

	while(ptr->next != NULL)
	{
		if(ptr->frequency < ptr->next->frequency)
		{
			fpointer temp = ptr->next;
			ptr->next = temp->next;
			ptr = temp;
		}
		ptr = ptr->next;
	}
}

int search(Files filenames, char *file) {
	if (strcmp(filenames->name, file) == 0) {
		return 0;
	}
		int count = 1;
		Files file_ptr = filenames;
		for (; file_ptr->next != NULL; file_ptr = file_ptr->next) {
			if (strcmp(file_ptr->name, file) == 0) {
				return count;
			} else {
				count++;
			}
		}
		count++;
	return count;
}

/*
 * Attempts to write into the given file pointer
 *  appends the words, file name, and frequency into the given file
 */
void print(Tree root, FILE * pointer, Files filenames) {
	
	fpointer ptr;
	if (root == NULL) {
		return;
	}
	if (root->left != NULL) {
		print(root->left, pointer, filenames);
	}
	
	fprintf(pointer, "<list> %s\n", root->token);
	ptr = root->files;
	sort_byFrequency(ptr);
	while (ptr != NULL) {
		printf("PrintToken: %s\n", root->token);
		printf("Filename: %s\n", ptr->filename);

		int filenum = search(filenames, ptr->filename);
		
		fprintf(pointer, "<%i %i>", filenum, ptr->frequency);
		ptr = ptr->next;
	}
	
	fprintf(pointer, "\n</list>\n");
	if (root->right != NULL) {
		print(root->right, pointer, filenames);
	}
	return;
}

/*
 * Attempts to write into the given file,
 *  appends the file names into the give file
 */
void print_file(FILE *ptr, Files filenames) {
	fprintf(ptr, "<files>\n");

	Files files_ptr = filenames;
	for (; files_ptr != NULL; files_ptr = files_ptr->next) {
		fprintf(ptr, "<name>%s</name>", files_ptr->name);
	}

	fprintf(ptr, "\n</files>\n");
}

/* 
 * Inserts a tree node into a tree. If the node is already present, 
 *  increase a frequency count in the tree rather than adding it 
 * In all cases, for the reaction of a new node, the following fields are set: 
 *  frequency is equal to 1, file is equal to whatever is passed in, 
 *  and everything else is null.*/
void insert_Tree(Tree root, char * term, char * file, Counter counter) {
	
	Tree insertion = treeCreate();
	insertion->token = calloc(strlen(term)+1, sizeof(char));
	strcpy(insertion->token, term);

	fpointer temp;

	if (root->token == NULL) {  /*if the tree is empty, make the insertion node the root node*/
		temp = fpointerCreate();
		temp->filename = file;
		temp->frequency = 1;
		insertion->files = temp;
		*root = *insertion;
		counter->word_count++;
		return;
	}
	
	if (strcasecmp(insertion->token, root->token) == 0) { /*if it's found it:*/
		increase(root->files, file);
		free(insertion->token);
		free(insertion);
	}
	else if (strcasecmp(insertion->token, root->token) < 0) {
		if (root->left == NULL) { 

		/*if the left ndoe isn't present, 
		but this node has to get there, then just place it there*/

			temp = fpointerCreate();
			temp->filename = file;
			temp->frequency = 1;
			insertion->files = temp;
			root->left = insertion;
			counter->word_count++;
			return;
		}
		else { /*otherwise just recur*/
			
			 insert_Tree(root->left, term, file, counter);
			 return;
		}
	}
	else {
		if (root->right == NULL) { /*same with the right node*/
			temp = fpointerCreate();
			temp->filename = file;
			temp->frequency = 1;
			insertion->files = temp;
			root->right = insertion;
			counter->word_count++;
			return;
		}
		else { /*the same*/
			insert_Tree(root->right, term, file, counter);
			return;
		}
	}
}

void freelist(fpointer head) {
	
	fpointer temp;
	
	if (head == NULL) {
		return;
	}
	while (head != NULL) {
	
		temp = head;
		if (head->next == NULL) {
			
			free(temp->filename);
			free(temp);
			break;
		}
		else {
			
			head = head->next;
			free(temp->filename);
			free(temp);
		}
	}
	return;
}

/*Recursively frees an entire tree*/
void freeup(Tree root) {
	
	if (root->left != NULL) {
		
		freeup(root->left);
	}
	if (root->right != NULL) {
		
		freeup(root->right);
	}
	
	free(root->token);
	/*freelist(root->files);*/
	free(root);
	return;
}
