
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include "tokenizer.h"

int main()
{
  FILE * fileptr;
  if ((fileptr = fopen("result.txt", "r")) == NULL) {
    return 0;
  }

  fseek(fileptr, 0L, SEEK_END);
  long lSIZE = ftell(fileptr);
  fseek(fileptr, 0L, SEEK_SET);

  char * buffer;
  if((buffer = (char *)calloc(lSIZE+1, sizeof(char))) == 0)
  {
    printf("Not enough memory");
    return 0;
  }

  fread(buffer, sizeof(char), lSIZE, fileptr);
  fclose(fileptr);
  printf("%s \n", buffer);
  TokenizerT x = TKCreate("<>", buffer);
  char * returnv;
  returnv = TKGetNextToken(x);
  
  printf("Output: %s \n", returnv);

  

	return 0;
}

