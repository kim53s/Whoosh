#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void reportError();
void checkCommand();
int getWords(char *string);

char *str;
char **command;

// number of words in a command
int num_words= 5;

int main(int argc, char **argv){

	if(argc > 1){
		reportError();	
		exit(1);
	}

	str = (char*)malloc(sizeof(char)*128);

	checkCommand();

	return 0;
	
}

int getWords(char *string){

   	const char s[2] = " ";
   	char *token;
   	int index = 0;

   	// the first token 
   	token = strtok(string, s);
  
   	while(token != NULL){
   		if(token[0] != '\n')
   			index++;
   		command[index-1] = token;
   		token = strtok(NULL, s);
   	}

   	printf("NUM TOKEN: %d\n", index);
   	return index;
}

void checkCommand(){
	command = (char**)calloc( num_words, sizeof(char*)*128 ); 

	printf("whoosh> ");

	fgets(str, 128, stdin);

	int numWords = getWords(str);
    
    if(numWords == 1){
    	if(strcmp(str, "exit\n") == 0){
			exit(1);
		}
		else if(strcmp(str, "pwd\n") == 0){
			char buffer[128];
			getcwd(buffer, 128);
			printf("%s\n", buffer);
		}
		else if(strcmp(str, "cd") == 0){
	        printf("COMMAND2: %s", command[1]);
			int i;
			i = chdir(getenv("HOME"));

			if(i == -1){
				reportError();
			}
		}
    }
    else if(numWords == 2){
    	if(strcmp(command[0], "cd") == 0 && numWords == 2){
	        int i;
	        command[1][strlen(command[1])-1] = 0;

	        i = chdir(command[1]);
	        if(i != 0){
	           reportError();
	    	}
	    }
    }

	free(command);

  	checkCommand();
}

void reportError() {
	char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message));
}
