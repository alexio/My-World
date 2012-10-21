/*
 * tokenizer.c
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tokenizer.h"

#define start 0
#define true 0
#define false 1


/*
 * Helper method to TKCreate(), checks for special characters.
 * The function takes in both current positions (int) in the two given string and the string itself (char*)
 *
 * If the next char is a special character, the function updates the copy string with the special character as one char and returns true, 
 * otherwise false.
 */
int check_special(char *copy, char *original, int copy_position, int ori_position) {
	if (ori_position + 1 > strlen(original)) { // if at the end of the string
		return false;
	}
	if (original[ori_position] == '\\') { // if current character is a backslash
		if (original[ori_position+1] == 'n') {
			copy[copy_position] = '\n';
		} else if (original[ori_position+1] == 't') {
			copy[copy_position] = '\t';
		} else if (original[ori_position+1] == 'v') {
			copy[copy_position] = '\v';
		} else if (original[ori_position+1] == 'b') {
			copy[copy_position] = '\b';
		} else if (original[ori_position+1] == 'r') {
			copy[copy_position] = '\r';
		} else if (original[ori_position+1] == 'f') {
			copy[copy_position] = '\f';
		} else if (original[ori_position+1] == 'a') {
			copy[copy_position] = '\a';
		} else if (original[ori_position+1] == '\\') {
			copy[copy_position] = '\\';
		} else if (original[ori_position+1] == '?') {
			copy[copy_position] = '\?';
		} else if (original[ori_position+1] == '\'') {
			copy[copy_position] = '\'';
		} else if (original[ori_position+1] == '\"') {
			copy[copy_position] = '\"';
		} else { // not a special character
			return false;
		}
		return true;
	}
	return false;
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
	// initializes the tokenizer struct
	TokenizerT tokenizer = calloc(3,sizeof(TokenizerT));
	if (tokenizer == NULL) { return NULL; }
	else {
		// allocates the memory for the separator and token string
		tokenizer->separatorPTR = calloc(strlen(separators),sizeof(char));
		tokenizer->tokenPTR = calloc(strlen(ts),sizeof(char));
		// position of the token stream is set to 0
		tokenizer->position = start;
		// special counter keeps track of the number of special characters
		int sep_copy_count = start, sep_count = start, tok_copy_count = start, tok_count = start, sep_special_count = start, tok_special_count = start;
		// loops and copies over the separators array
		for (; sep_copy_count < strlen(separators); sep_copy_count++, sep_count++) {
			// calls for helper, if special character found, increment special counter and the separators counter
			if (check_special(tokenizer->separatorPTR, separators, sep_copy_count, sep_count) == true) {
				sep_special_count++;
				sep_count++;
			} else {
				tokenizer->separatorPTR[sep_copy_count] = separators[sep_count];
			}
		}
		tokenizer->separatorPTR[strlen(separators)-sep_special_count] = '\0';
		// loops and copies over the token stream array
		for (; tok_copy_count < strlen(ts); tok_copy_count++, tok_count++) {
			// calls for helper, if special character found, increment special counter and the separators counter
			if (check_special(tokenizer->tokenPTR, ts, tok_copy_count, tok_count) == true) {
				tok_special_count++;
				tok_count++;
			} else {
				tokenizer->tokenPTR[tok_copy_count] = ts[tok_count];
			}
		}
		tokenizer->tokenPTR[strlen(ts)-tok_special_count] = '\0';
		
		/*printf("================================\n");
		printf("Separators: \"%s\"\n", tokenizer->separatorPTR);
		printf("Token Stream: \"%s\"\n", tokenizer->tokenPTR);
		printf("================================\n\n");*/
		return tokenizer;
	}
}

/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * You need to fill in this function as part of your implementation.
 */
void TKDestroy(TokenizerT tk) {
	// frees the memory of the separators array
	free(tk->separatorPTR);
	// frees the memory of the token stream array
	free(tk->tokenPTR);
	// fres the tokenizer struct itself
	free(tk);
}

/*
 * Helper method to TKGetNextToken(), checks for separators.
 * The validate() method requires the struct Tokenizer_T*,
 * and the position in the token stream array to check.
 *
 * If the function succeeds in validating that it is a separator, it returns a 0.
 * Else it returns 1.
 */
char validate(TokenizerT tk, int position) {
	int i;
	// loops through the separator string,
	// comparing each (char) in the separator string to the specified token (char)
	for (i = start; i < strlen(tk->separatorPTR); i++)
		if (tk->tokenPTR[position] == tk->separatorPTR[i])
			return true;
	return false;
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
	// checks if the current position in the token stream is a separator
	if (validate(tk, tk->position) == true) {
		return true;
	} else { // if the current position of the token stream is not a separator
		int i, n = start, end = tk->position;
		// loops and increment the counter (end) until the next separator
		for (i = (tk->position)+1; i < strlen(tk->tokenPTR); i++, end++) {
			if (validate(tk, i) == true) { // calls helper to check for tokens
				break;
			}
		}
		// allocates memory equal to the total size of the token stream minus the current position
		char *token = calloc(end-(tk->position)+1,sizeof(char));
		// loops through the token stream and copies over the recorded position of the token in the token stream
		for (i = (tk->position); i <= end; i++, n++) {
			token[n] = tk->tokenPTR[i];
		}
		// updates the position of the token stream to the position (end) of the last known separator
		tk->position = end;
		return token;
	}
}

/*
 * main will have two string arguments (in argv[1] and argv[2]).
 * The first string contains the separator characters.
 * The second string contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 *
 *
int main(int argc, char **argv) {
	// checks if the user entered the correct format
	if (argc != 3)
	{
		printf("**You have entered the incorrect format, please try again** \n");
		return false;
	}
	// stores the first input (separators)
	char *separators = argv[1];
	// stores the second input (token stream)
	char *ts = argv[2];
	// copies the separator string and the token stream string
	TokenizerT tokenizer = TKCreate(separators, ts);
	
	while (tokenizer->position < strlen(tokenizer->tokenPTR)) {
		//printf("Total before get token: \"%zi\"\n", strlen(tokenizer->tokenPTR));
		char *token = TKGetNextToken(tokenizer);
		// only printf if the return value of char* from TKGetNextToken() != 0
		if (token != NULL) {
			printf("\"%s\"\n", token);
		}
		// frees the memory allocated during TKGetNextToken()
		free(token);
		// increment the position, since TKGetNextToken() stops at the last known position of a separator in the token stream
		tokenizer->position++;
	}
	// calls TKDestroy() to free the memory allocated during TKCreate()
	TKDestroy(tokenizer);
	return true;
}
*/
