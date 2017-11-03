#include <string.h>
#include <stdio.h>

int main(){

	int val = 0;
	char *line = NULL;
	size_t size = 0;

	printf("Please start your input: \n");

	//WHILE NOT END OF FILE
	//while getline != null?

	//parse each line of user's input, until they ctrl+d
	getline(&line, &size, stdin);
	
	// ignores spaces in beginning of line
	while(*line == ' '){
		
		line++;  // increment index of character array
		if(*line == '-'){  // if the character is a hyphen
			line++;  // increment index of character array
	
			if(*line == 'c'){  // if next character is c

				line++;  // increment index of character array

				// ignore all spaces after "-c"
				while(*line == ' '){
					line++;
				}
		
				// print all characters after "-c"
				printf("%s", line);

			}
	
			else if(*line == 'd'){
				
				line++;  // increment index of character array
				
				char* token = strtok(line, " \n");  // splits string by space and new line
				while(token != NULL){
					sscanf(token, "%d", &val);  // stores numbers in val
					printf("%c", val);  // converts int to character
					token = strtok(NULL, " \n");  // re-assigns token to new delimited string
				}
			}

			else{  // assume hexadecimal
				
                                char* token = strtok(line, " \n");  // splits string by space and new line
                                while(token != NULL){
                                        sscanf(token, "%d", &val);  // stores numbers in val
                                        printf("%c", val);  // converts int to character
                                        token = strtok(NULL, " \n");  // re-assigns token to new delimited string
				}				
				
				sscanf(token, "%x", &val);  // converts hexadecimal to character

			}
		}
		
		

	}	
	
	

	printf("end\n");
	

}
