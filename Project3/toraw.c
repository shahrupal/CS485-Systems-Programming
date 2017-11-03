#include <string.h>
#include <stdio.h>

int main(){

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
		}
		
		

	}	
	
	

	printf("end\n");
	

}
