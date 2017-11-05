// Author: Rupal Shah

#include <string.h>
#include <stdio.h>
#include <ctype.h>

int main() {


	/* GENERAL COMMENTS:
		-- 'line' is an array of characters (AKA a string)
		-- 'line++' increments the index of the array being pointed to
		-- '*line' dereferences the current character being pointed to
		-- 'token' is a string of characters; each token is separated by spaces or a new line (ex: ab bc cd -> the first token is "ab")
	 */

	// used to store each line of input 
	char *line = NULL;
	size_t size = 0;

	// used to store string of output
	char output[1024] = "";
	size_t len = 0;
	
	// set errors to false
	int decError = 0;
	int hexError = 0;

	// create strings to hold which input value is giving error
	char *decErrorMsg = NULL;
	char *hexErrorMsg = NULL;

	// used to store decimal/hex values
	int val = 0;

	// prompt user to input
	printf("Please start your input: \n");

	// allow user to input lines until they press ctrl+d
	// parse each line of user's input and store in 'line'
	while(getline(&line, &size, stdin) != -1) {

		// ignores spaces in beginning of line
		while(*line == ' ') {
			line++;
		}

		// if character is hyphen
		if(*line == '-') { 
			line++;  
				
			// ignore all spaces after hyphen
			while(*line == ' ') {
				line++;
			}
	
			// if next character is 'c'
			if(*line == 'c') { 

				line++;
	
				// ignore all spaces after '-c'
				while(*line == ' ') {
					line++;
				}

				// ignore comments
				char* token = strtok(line, "\n");  // separate line by new lines ONLY
				for(int i = 0; i < strlen(token); i++){  // iterate through each character in token (line)
					if(token[i] == '/' && token[i + 1] == '*'){  // if begining of comment is detected
						while(token[i] != '*' || token[i + 1] != '/') {  // until end of comment
							i++;  // do nothing (ignore characters in between)
						}
					i += 2;  // account for '*/'
					}

					// add characters to output string
					output[len++] = token[i];
					output[len] = '\0';

				}

			}
	
			// if character after hyphen is 'd'
			else if(*line == 'd') {
					
				line++;  
					
				// split line into substring tokens (splits by space or new line)
				char* token = strtok(line, " \n");

				// until end of line
				while(token != NULL) {
	
					// ignore comments
					if(token[0] == '/' && token[1] == '*') {  // if beginning of comment is detected

						token = strtok(NULL, " \n");  // go to next token
						while(token[0] != '*' && token[1] != '/') {  // until token is end of comment
							token = strtok(NULL, " \n");  // go to next token
						}
				
						token = strtok(NULL, " \n");  // goes to next token after comment (if applicable)
						
					}
					
					if(token != NULL) {  // accounts for if comment ends at end of line (if not at end of line...)

						// iterate through each character of current token
						for(int i = 0; i < strlen(token); i++) {
							if(!isdigit(token[i])){  // if token includes non-digit
								decError = 1;  // store that there is an error with the decimal input
								decErrorMsg = token;  // store token that includes non-digit character
							}
						}

						sscanf(token, "%d", &val);  // converts strings of numbers into actual integers
						output[len++] = (char)val;  // converts integers into characters (and stores result in output string)
						output[len] = '\0';
						token = strtok(NULL, " \n");  // re-assigns token to new delimited string
					}
				}
			}
		}
	
		else {  // assume hexadecimal if no hyphen
				
			// split line into substring tokens (splits by spaces or new line)		
			char* token = strtok(line, " \n");  

			// until end of line
       		        while(token != NULL){
				
				// ignores comments
                		if(token[0] == '/' && token[1] == '*') {  // if beginning of comment is detected

					token = strtok(NULL, " \n");  // go to next token
					while(token[0] != '*' && token[1] != '/') {  // until token is end of comment
						token = strtok(NULL, " \n");  // go to next token
					}

					token = strtok(NULL, " \n");  // goes to next token after comment (if applicable)

				}

				if(token != NULL){  // accounts for if comment ends at end of line (if not at end of line...)
			
					// iterate through each character of current token
					for(int i = 0; i < strlen(token); i++) {

						// if the character is not a digit, nor a letter, OR is notof length two
						if((!isdigit(token[i]) && !isalpha(token[i])) ||  (strlen(token) != 2)) {
							hexError = 1;  // store that there is an error with the hexadecimal input
							hexErrorMsg = token;  // store token with error
						} 
					}				
	
					sscanf(token, "%x", &val);  // converts string of numbers/letters into actual hexadecimal values
					output[len++] = (char)val;  // converts hex into characters (and stores them in output string)
					output[len] = '\0';
     			                token = strtok(NULL, " \n");  // re-assigns token to new delimited string
				}
		
			}								
		}	
	}
	
	// AFTER ALL LINES HAVE BEEN READ
	// if there are errors, output the error and which decimal/hexadecimal caused the error
	if(decError == 1 && hexError == 1) {
		printf("ERROR: %s is not a decimal number.", decErrorMsg); 
		printf("ERROR: %s is not a hexadecimal number.", hexErrorMsg);
	}	
	else if(decError == 1) {
		printf("ERROR: %s is not a decimal number.", decErrorMsg);
	}
	else if(hexError == 1) {
		printf("ERROR: %s is not a hexadecimal number.", hexErrorMsg);
	}
	// if there are no errors, print all conversion of characters, decimals, and hexadecimals given into characters on same line
	else {
		for(int k = 0; k < 1024; k++) {
			printf("%c", output[k]);
		}
	}

	printf("\n");
	
}
