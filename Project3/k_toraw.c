#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

char *trimwhitespace(char *str)
{
    char *end;
    
    // Trim leading space
    while(isspace((unsigned char)*str)) str++;
    
    if(*str == 0)  // All spaces?
        return str;
    
    // Trim trailing space
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    
    // Write new null terminator
    *(end+1) = 0;
    
    return str;
}

int hex_to_int(char c)
{
    if (c >= 97)
    {
        c = c - 32;
    }
    int first = c / 16 - 3;
    int second = c % 16;
    int result = first * 10 + second;
    if (result > 9) result--;
    return result;
}

int hex_to_ascii(char c, char d){
    int high = hex_to_int(c) * 16;
    int low = hex_to_int(d);
    return high+low;
}



int main () {
    
    char input[1025];
    char output[1025];
    char *token;
//    FILE *fp = fopen("output.txt", "w");
    
    while (fgets(input, 1025, stdin) != NULL)
    {
        char line[1025] = "";
        token = strtok(input, " \n");
        
        if (token == NULL) {
            continue;
        }
        else if (strcmp(token, "-c") == 0) {
            token = strtok(NULL, "\n");
            
            int a, i = 0;
            for(a = 0; a < strlen(token); a = a + 1 ) {
                if (isspace(token[a])) i = i + 1;
                else break;
            }
            
            memmove(token, token+i, strlen(token)); //remove initial white space
            strcat(line, token);
            printf("%s", line);
        }
        else if (strcmp(token, "-d") == 0) {
            token = strtok(NULL, " \n");
            while(token != NULL)
            {
                if (token[strspn(token, "0123456789")] != 0){
                    strcpy(line,  "Error: ");
                    strcat(line, token);
                    strcat(line, " is not a decimal number.");
                }
                else{
                    int len = strlen(line);
                    char c = (char) atoi(token);
                    line[len] = c;
                    printf("%s", line);
                }
                token = strtok(NULL, " \n");
            }
        }
        else{
            while(token != NULL)
            {
                if (token[strspn(token, "0123456789abcdefABCDEF")] != 0){
                    strcpy(line,  "Error: ");
                    strcat(line, token);
                    strcat(line, " is not a hexadecimal number.");
                }
                else{
                    printf("%c", hex_to_ascii(token[0], token[1]));
                }
                token = strtok(NULL, " \n");
            }
        }
    }
    
    return(0);
}

