/*
 * tokenizer.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokenizer.h"

/*
 * Tokenizer type.  You need to fill in the type as part of your implementation.
 */

/* copies string arguments and also changes the escape sequences to the appropriatye special char*/
char * specialCopy(char * arg_string){
 
	int i, j;
    i = j = 0;
	char * result;
	if((result = (char*) calloc(strlen(arg_string)+1, sizeof(char))) == 0){
	
		return NULL;
	}
    
    while (arg_string[i] != '\0') {
		
		if(arg_string[i] == '\\')
		{
			/*  We've found an escape sequence, so translate it  */
			i++;
			if(arg_string[i] == 'n')
			{	
				result[j] = '\n';
			}
			else if(arg_string[i] == 't')
			{
				 result[j] = '\t';
			}
			else if(arg_string[i] == 'v')
			{
				result[j] = '\v';
			}
			else if(arg_string[i] == 'b')
			{
				 result[j] = '\b';
			}
			else if(arg_string[i] == 'r')
			{
				result[j] = '\r';
			}
			else if(arg_string[i] == 'f')
			{
				result[j] = '\f';
			}
			else if(arg_string[i] == 'a')
			{
				result[j] = '\a';
			}
			else if(arg_string[i] == '\\')
			{
				result[j] = '\\';
			}
			else if(arg_string[i] == '\?')
			{
				result[j] = '\?';
			}
			else if(arg_string[i] == '\'')
			{
				result[j] = '\'';
			}
			else if(arg_string[i] == '\"')
			{
				result[j] = '\"';
			}
			else
			{
				/*  We don't translate this escape
                    sequence, so just copy it verbatim  */
				result[j++] = '\\';
                result[j] = arg_string[i];
			}
		}
		else
		{
			 /*  Not an escape sequence, so just copy the character  */
            
            result[j] = arg_string[i];
		}
        i++;
        j++;
    }
	result[j] = '\0';
	return result;
 }
 
/*
 * TKCreate creates a new TokenizerT object for a given set of separator
 * characters (given as a string) and a token stream (given as a string).
 * 
 * TKCreate should copy the two arguments so that it is not dependent on
 * them staying immutable after returning.  (In the future, this may change
 * to increase efficiency.)
 *
 * If the function succeeds, it returns a non-NULL TokenizerT.
 * Else it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */

TokenizerT TKCreate(char *separators, char *ts) {

	TokenizerT token_stream; /*tokenizer*/
	
	if(( (token_stream = calloc(1, sizeof(struct TokenizerT_))) == 0))
	{
		return NULL;
	}
	
	token_stream->stream = specialCopy(ts); /* points to copy of token stream that have special chars as one char*/
	token_stream->delims = specialCopy(separators); /* points to copy of separators that have special chars as one char*/
	token_stream->position = 0; /*at position 0 token_stream->stream*/
	
	return token_stream;
}

/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * You need to fill in this function as part of your implementation.
 */

void TKDestroy(TokenizerT tk) {

	free(tk->delims);
	free(tk->stream);
	free(tk);
}

/*
 * TKGetNextToken returns the next token from the token stream as a
 * character string.  Space for the returned token should be dynamically
 * allocated.  The caller is responsible for freeing the space once it is
 * no longer needed.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */
 
 

char *TKGetNextToken(TokenizerT tk) {

	int found_sep, i, pt, start; /*the variable (found_sep) keeps track of when a separated is identified, the variable 'i' keeps track of the end of the current token*/
	char * token; 
	
	found_sep = 0;
	for(i = tk->position; tk->stream[i] != '\0' ; i++)
	{	
		int j;
		for(j = 0; tk->delims[j] != '\0'; j++) /*check to see if current char is a separator*/
		{
			if(tk->stream[i] == tk->delims[j]) /*locates separators in token stream*/
			{
				found_sep = 1;
				break;
			}
		}
		if(found_sep) /*if separator was found*/
		{
			if((i - tk->position) == 0) /*if size of token is zero, then it's because only a separator was found since the start of the search, increment tk->position so that it will be the starting index of a token and not a separator*/
			{
				tk->position++;
				found_sep = 0;
			}else{ /*if variables i != tk->position, then a token has been found. "tk->position" is the starting index of the token while "i" is the ending index*/
				break;
			}
		}
	}
	
	if((i - tk->position) == 0){ /*no token, just separator chars*/
		return 0;
	}
	
	if((token = calloc((i-tk->position + 1), sizeof(char))) == 0){
		return 0;
	}
	
	pt = 0; /*index for token array*/
	for(start = tk->position; start < i; start++)
	{
		token[pt++] =  tk->stream[start]; /*copies token from stream to token variable*/
	}
	
	tk->position = i;
	
	return token;
}

/*
 * main will have two string arguments (in argv[1] and argv[2]).
 * The first string contains the separator characters.
 * The second string contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 */
