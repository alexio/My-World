
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include "index.h"

/*allocatoes memory for tree node*/
Tree treeCreate() {
	
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

/*allocates memory for filepointer node*/
fpointer fpointerCreate() {
	
	fpointer ans;
	if ((ans = calloc(1, sizeof(struct filepointer))) == 0) {
		exit(1);
	}
	ans->filename = NULL;
	ans->frequency = NULL;
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
			temp->filename = file;
			temp->frequency++;
			ptr->next = temp;
			return;
		}
	}
}

/*sorts files*/
void sortFile(fpointer ptr, fpointer newOne){
	
	fpointer point;
	if(ptr == 0 || ptr->next == 0){
		
			return;
	}
	point = newOne;
	while(point->next != 0){
		
		if(ptr->frequency > point->next->frequency ||
		 ptr->frequency == point->next->frequency){
				
			fpointer temp = ptr;
			ptr = ptr->next;
			temp->next = point->next;
			point->next = temp->next;
			break;
		}
		point = point->next;
	}
}

/*sorts files in which a term appears by frequency*/
void sort_byFrequency(fpointer point){
	
	fpointer newOne;
	fpointer temp;
	fpointer ptr;
	if(point == 0 || point->next == 0){
		
			return;
	}
	newOne = point;
	
	ptr = point->next;
	
	
	newOne->next = NULL;
	
	while(ptr != NULL){
		
		if(ptr->frequency > newOne->frequency){
			
				temp = ptr;
				ptr = ptr->next;
				temp->next = newOne;
				newOne = temp;
		}
		else { /*(ptr->frequency < newOne->frequency)*/
			
			sortFile(ptr, newOne);
		}
	}
	point = newOne;
}

void print(Tree root, FILE * pointer) {
	
	fpointer ptr;
	if (root == 0) {
		return;
	}
	if (root->left != 0) {
		print(root->left, pointer);
	}
	
	fprintf(pointer, "<list> %s\n", root->token);
	ptr = root->files;
	sort_byFrequency(ptr);
	
	while (ptr != 0) {
		fprintf(pointer, "%s %d ", ptr->filename, ptr->frequency);
		ptr = ptr->next;
	}
	
	fprintf(pointer, "\n</list>\n");
	if (root->right != 0) {
		print(root->right, pointer);
	}
	return;
}

/*Inserts a tree node into a tree. If the node is already present, 
 * increase a frequency count in the tree rather than adding it 
 * In all cases, for the reaction of a new node, the following fields are set: 
 * frequency is equal to 1, file is equal to whatever is passed in, 
 * and everything else is null.*/
void insert(Tree root, char * term, char * file) {
	
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
			return;
		}
		else { /*otherwise just recur*/
			
			 insert(root->left, term, file);
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
			return;
		}
		else { /*the same*/
			insert(root->right, term, file);
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
