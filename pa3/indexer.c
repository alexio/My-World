#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include "index.h"
#include "hash.h"


void recurseDir(hashTable tbl, struct dirent direc)
{
	char * temp2;
	char* name;
	DIR * dir = NULL;
	struct dirent * entry;
	dir = opendir(directory); /*this is assuming that the character in 'directory' 
	is a legit directory. It should never be an actual file, 
	* because this is only called when fd has returned it as a directory.*/
	while ((entry = readdir(dir)) != NULL) { 
		/*scan through everything in the parent directory*/
		name = entry->d_name;
		if ((name = (char *)malloc(sizeof(char) * strlen(entry->d_name) + 1)) == 0) {
			printf("AAAAAAH");
			exit(1);
		}
		strcpy(name, entry->d_name);
		if (strcasecmp(name, ".") == 0 || strcasecmp(name, "..") == 0) { /*checks to see if it's trying to call it on the current or parent directory again, so just stops it.*/
			free(name);
			continue;
		}
		
		temp2 = (char *)malloc(sizeof(char) * (strlen(name) + strlen(directory) + 2));
		strcpy(temp2, directory);
		strcat(temp2, "/");
		strcat(temp2, name);
		strcat(temp2, "\0"); /*this has created a string that is the relative 
		pathname to the current file/directory that is being worked on. 
		* This will be generated for every file, 
		* even the ones in the relative home directory.*/
		if (fd(temp2) == 1) {
			directrecur(temp2, tokentree);
			free(temp2);
		}
		else {
			printf("Scanning %s in %s\n", name, directory);
			filescan(tokentree, temp2);
		}
		free(name);
	}
}

void filescan(hashTable tvl, struct dirent direc)
{

}