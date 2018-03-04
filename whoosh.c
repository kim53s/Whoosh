#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void reportError();
void checkCommand();
void pwd();
void cd();
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

void checkCommand(){

	command = (char**)calloc( num_words, sizeof(char*)*128 ); 

	printf("whoosh> ");

	// get user input
	fgets(str, 128, stdin);

//	int numWords = getWords(str);
	getWords(str);

	if(command[0][(int)strlen(command[0])-1] == '\n'){
		command[0][(int)strlen(command[0])-1] = '\0';
	}

    if(strcmp(command[0], "exit") == 0){
		exit(0);
	}
	else if(strcmp(command[0], "pwd") == 0){
		pwd();
	}
	else if(strcmp(command[0], "cd") == 0){
		cd();
	}

	free(command);

  	checkCommand();
}

//print working directory
void pwd(){
	char buffer[128];
	getcwd(buffer, 128);
	printf("%s\n", buffer);
}

// change working directory
void cd(){
	if(command[1] != NULL && command[1][0] != '\n'){
	    int i;

	    command[1][strlen(command[1])-1] = 0;

	    i = chdir(command[1]);
		if(i == -1){
	       reportError();
		}
	}
	else{
		int i;
		i = chdir(getenv("HOME"));

		if(i == -1){
			reportError();
		}
	}	
}

int getWords(char *string){

   	const char s[2] = " ";
   	char *token;
   	int index = 0;

   	// the first token 
   	token = strtok(string, s);

   	while(token != NULL){
   		command[index] = token;
   		token = strtok(NULL, s);
   		if(token != NULL)
   			index++;
   	}

//   	printf("NUM TOKEN: %d\n", index);
   	return index;
}

void reportError() {
	char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message));
}