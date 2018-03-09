#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>

void reportError();
void checkCommand();
void pwd();
void cd();
void setPath();
void printPath();
void exec(char *file, char *path);
void execWithPath(char str[]);
int parseCommand(char *string);
int searchFile(char *file, char *path);

// the maximun length of command line
int const MAX_COMMAND = 128;
char *line;
char **command;
char **currentPath;
int numPaths=1;

int main(int argc, char **argv){

	if(argc > 1){ 
		reportError();	
		exit(1);
	}

	// stores user input
	//line = (char*)malloc(sizeof(char) * MAX_COMMAND);
	currentPath = (char**)calloc( MAX_COMMAND, sizeof(char*) * MAX_COMMAND );
	// default path
	currentPath[0] = "/bin";

	checkCommand(argc);

	return 0;
}

// recursively get user input and check if it is a valid command
void checkCommand(){

	command = (char**)calloc( MAX_COMMAND, sizeof(char*) * MAX_COMMAND ); 
	line = (char*)malloc(sizeof(char) * MAX_COMMAND);

	printf("whoosh> ");

	// get user input
	fgets(line, MAX_COMMAND+1, stdin);
	//free(line);

	// if the input is over 128 bytes, then report error
	if(strlen(line) >= 128){
		printf("LINE: %s\n", line);
		reportError();
	}
	else{
		// number of words in the command line
		int i = parseCommand(line);

		// if the last character is the newline, then delete it
		if(command[0][(int)strlen(command[0])-1] == '\n'){
			command[0][(int)strlen(command[0])-1] = '\0';
		}

	   	 if(strcmp(command[0], "exit") == 0){
	    	free(line);
	    	free(command);
	    	free(currentPath);
			exit(0);
		}
		// print working directory
		else if(strcmp(command[0], "pwd") == 0){
			pwd();
		}
		// change directory
		else if(strcmp(command[0], "cd") == 0){
			cd();
		}
		else if(strcmp(command[0],"setpath") == 0){

			if(i >= 1){
				numPaths = i;

				setPath();
			}
			else{ // if path is not passed
				reportError();
			}
		}
		else if(strcmp(command[0],"printpath") == 0){
			printPath();
		}
		else{
			// search for executable program in the current path
			int result = 0;
			for(int i = 0; i < numPaths; i++){
				// if there exists an executable program, then execute it
				if(searchFile(command[0], currentPath[i]) == 1){
					exec(command[0], currentPath[i]);
					result++;
					break;
				}
			}
			if(result == 0){  // the file is not found in any path
				printf("NO SUCH COMMAND\n");
				reportError();
			}
		}
	}
	free(command);
	free(line);
	
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

		if(command[1][(int)strlen(command[1])-1] == '\n'){
			command[1][(int)strlen(command[1])-1] = '\0';
		}

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

void setPath(){
	
    for(int i=0;i<numPaths;i++){ //start with one path
 		currentPath[i]=(char *)malloc(strlen(command[i+1])+1);
       	strcpy(currentPath[i], command[i+1]);

       	// if the last character is '\n', then delete it
   		if(currentPath[i][(int)strlen(currentPath[i])-1] == '\n'){
			currentPath[i][(int)strlen(currentPath[i])-1] = '\0';
		}
    }
}

void printPath(){
	for(int i = 0; i < numPaths; i++){
		printf("%s\n", currentPath[i]);
	}
}

void exec(char *file, char *path){
	int status;
    char *args[2];
		
	char str[strlen(file)+strlen(path)+1];
	strcpy(str, path);
	strcat(str, "/");
	strcat(str, file);
	args[0] = str;       
	args[1] = NULL;          

	int pid = fork();
	  	  
	if(pid == 0){ // child
	    execv( args[0], args ); 
	}
	else if(pid > 0){ // parent
		wait( &status );  
	}
	else { 
	    reportError();
	}
}

int parseCommand(char *string){

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

   	return index;
}
//changed to search every path
// return 1 if there exists the file in the current path
int searchFile(char *file, char *path) {
    int result = 0;
    struct stat buf;
   
	char str[strlen(file)+strlen(path)+1];

	//if the last character is a new line character, delete it
	if(path[(int)strlen(path)-1] == '\n'){
		path[(int)strlen(path)-1] = '\0';
	}

	strcpy(str,path);
	strcat(str, "/");
	strcat(str, file);
	    
	if(stat(str, &buf) == 0){
	    result = 1;
	}

    return result;
}

void execWithPath(char str[]){
	int status;
	char *args[2];
	args[0]=str;
	args[1]=NULL;
	int pid=fork();
	if(pid == 0){
		execv(args[0],args);
	}
	else if(pid > 0){
		wait( &status );
	}
	else{
		reportError();
	}
}

void reportError() {
	char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message));
}