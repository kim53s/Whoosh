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
void exec(char *string);
int parseCommand(char *string);
int searchFile(char *file);

// the maximun length of command line
int const MAX_COMMAND = 128;
//int numPaths;
char *line;
char **command;
char **path;
//TODO: figure out exact number of arguments in the command
// number of words in a command
int num_words= 128;
int num_args=0;

int main(int argc, char **argv){
	num_args=argc;
	if(argc > 1){
		reportError();	
		exit(1);
	}

	line = (char*)malloc(sizeof(char) * MAX_COMMAND);
	path = (char**)calloc( num_words, sizeof(char*) * MAX_COMMAND );
	path[0] = "/bin";
	//numPaths=1;
	checkCommand();

	return 0;
	
}

void checkCommand(){

	command = (char**)calloc( num_words, sizeof(char*) * MAX_COMMAND ); 
	
	
	printf("whoosh> ");

	// get user input
	fgets(line, 128, stdin);

	parseCommand(line);
	//free(line);

	if(command[0][(int)strlen(command[0])-1] == '\n'){
		command[0][(int)strlen(command[0])-1] = '\0';
	}

   	 if(strcmp(command[0], "exit") == 0){
    		free(line);
    		free(command);
    		free(path);
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
	//	if(num_args>2){
			
			setPath();
	//	}
	//	else{
	//		return;
	//	}
	}
	else{
	//sth wrong with searchFile
		printf("%s\n", command[0]);
		if(searchFile(command[0]) == 1){
			exec(command[0]);
		}
		else{
			reportError();
		}

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
	
                 for(int i=0;i<1;i++){ //start with one path
			path[i]=(char *)malloc(strlen(command[i+1])+1);
                        strcpy(path[i], command[i+1]);
			printf("The path is %s\n", path[i]);
                 }

}

void exec(char *string){
	int status;
    char *args[2];
		
	char str[strlen(string)+strlen(path[0])+1];
	strcpy(str, path[0]);
	strcat(str, "/");
	strcat(str, string);
   
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
int searchFile(char *file) {
	printf("file is %s\n", file);
    int result = 0;
    struct stat buf;
    for(int i=0;i<1;i++){
	printf("current path is %s\n",path[i]);
   	char str[strlen(file)+strlen(path[i])+1];
	strcpy(str, path[i]);
	strcat(str, "/");
	strcat(str, file);
	printf("full path is %s", str);
    	if(stat(str, &buf) == 0){
    		result = 1;
    	}

    return result;
}
}

void reportError() {
	char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message));
}
