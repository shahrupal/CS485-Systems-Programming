#include <string.h>
#include <stdio.h>
#include <ctype.h>

int main() {

	int val = 0;
	char *line = NULL;
//	char output[1024];  // array of strings
	int index = 0;
	size_t size = 0;
	int charDone = 0;

	// set errors to false
	int decError = 0;
	int hexError = 0;	
	char output[1024] = "";
	size_t len = 0;

	printf("Please start your input: \n");


	// ALLOWS USER TO KEEP GOING UNTIL CTRL - D
	// parse each line of user's input
	while(getline(&line, &size, stdin) != -1){

		// ignores spaces in beginning of line
		while(*line == ' ') {
			line++;  // increment index of character array
		}

		if(*line == '-') {  // if the character is a hyphen
			line++;  // increment index of character array
				
			// ignore all spaces after -
			while(*line == ' '){
				line++;
			}
	
			if(*line == 'c') {  // if next character is c

				line++;  // increment index of character array
	
				// ignore all spaces after "-c"
				while(*line == ' ') {
					line++;
				}
	
	
				// COMMENTS		
			
				// print all characters after "-c"
				printf("%s", line);

		}
	
			else if(*line == 'd') {
					
				line++;  // increment index of character array
					
				char* token = strtok(line, " \n");  // splits string by space and new line
				while(token != NULL) {
	
					// ignore comments
					if(token[0] == '/' && token[1] == '*'){
						token = strtok(NULL, " \n");
						while(token[0] != '*' && token[1] != '/'){
							token = strtok(NULL, " \n");
						}
						token = strtok(NULL, " \n");
					}

					// print error if input is not a digit
					for(int i = 0; i < strlen(token); i++){
						if(!isdigit(token[i])){
//							printf("ERROR: NOT DECIMAL");
							decError = 1;
						}
					}

					sscanf(token, "%d", &val);  // stores numbers in val
//					printf("%c", val);  // converts int to character
					output[len++] = (char)val;
					output[len] = '\0';
					token = strtok(NULL, " \n");  // re-assigns token to new delimited string
				}
				printf("\n");
			}
		}
	
		else {  // assume hexadecimal if no hyphen
				
			char* token = strtok(line, " \n");  // splits string by space and new line
       		        while(token != NULL){
				
				// ignores comments
                		if(token[0] == '/' && token[1] == '*'){
					token = strtok(NULL, " \n");
					while(token[0] != '*' & token[1] != '/'){
						token = strtok(NULL, " \n");
					}
					token = strtok(NULL, " \n");
				}

				// print error is input is not a digit or letter (not hex - must be 2 digits/letters)
				for(int i = 0; i < strlen(token); i++){
					if((!isdigit(token[i]) && !isalpha(token[i])) ||  (strlen(token) != 2)){
//						printf("ERROR: NOT HEX");
						hexError = 1;
					}
				}				
	
				sscanf(token, "%x", &val);  // stores numbers in val
//	                  	printf("%c", val);  // converts hex to character
				output[len++] = (char)val;
				output[len] = '\0';
     		                token = strtok(NULL, " \n");  // re-assigns token to new delimited string

			}		

			printf("\n");						

		}
		
	}
		
	for(int k = 0; k < 1024; k++){
		printf("%c", output[k]);
	}

	printf("\nend\n");
	

}
