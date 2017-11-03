#include <string.h>
#include <stdio.h>

int main() {

	int val = 0;
	char *line = NULL;
	size_t size = 0;
	int charDone = 0;

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
	
					sscanf(token, "%d", &val);  // stores numbers in val
					printf("%c", val);  // converts int to character
					token = strtok(NULL, " \n");  // re-assigns token to new delimited string
				}
			}
		}
	
		else {  // assume hexadecimal if no hyphen
				
			char* token = strtok(line, " \n");  // splits string by space and new line
       		        while(token != NULL){
                		sscanf(token, "%x", &val);  // stores numbers in val
	                  	printf("%c", val);  // converts hex to character
       		                token = strtok(NULL, " \n");  // re-assigns token to new delimited string
			}						
		

		}
		
	}
		printf("\nend\n");
	

}
