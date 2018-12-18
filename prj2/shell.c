// Shell file
// Provided by Prof. Keval Vora. Modified by Khang Vu.

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>

#define COMMAND_LENGTH 1024
#define NUM_TOKENS (COMMAND_LENGTH / 2 + 1)
#define HISTORY_DEPTH 10

char history[HISTORY_DEPTH][COMMAND_LENGTH]; //with form {command #, command}
int command_count = 0; 						 //to keep track of number of history command



/**
 * Command Input and Processing
 */

/*
 * Tokenize the string in 'buff' into 'tokens'.
 * buff: Character array containing string to tokenize.
 *       Will be modified: all whitespace replaced with '\0'
 * tokens: array of pointers of size at least COMMAND_LENGTH/2 + 1.
 *       Will be modified so tokens[i] points to the i'th token
 *       in the string buff. All returned tokens will be non-empty.
 *       NOTE: pointers in tokens[] will all point into buff!
 *       Ends with a null pointer.
 * returns: number of tokens.
 */
int tokenize_command(char *buff, char *tokens[])
{
	int token_count = 0;
	_Bool in_token = false;
	int num_chars = strnlen(buff, COMMAND_LENGTH);
	for (int i = 0; i < num_chars; i++) {
		switch (buff[i]) {
		// Handle token delimiters (ends):
		case ' ':
		case '\t':
		case '\n':
			buff[i] = '\0';
			in_token = false;
			break;

		// Handle other characters (may be start)
		default:
			if (!in_token) {
				tokens[token_count] = &buff[i];
				token_count++;
				in_token = true;
			}
		}
	}
	tokens[token_count] = NULL;
	return token_count;
}

//add command in buff to history
void add_history(char *buff){

	//if buff has command "!" > do not store
	//if buff is empty > do not store
	if (buff[0]== '!'){return;}
	if (buff[0]== '\0'){return;}


	command_count++;
	for (int i = 0; buff[i] != '\0'; i++){
		history[(command_count % 10)][i] = buff[i];
		history[(command_count % 10)][i+1] = '\0';
	}

	return;
}


void print_history(void){

	//If there has been less than 10 commands given, simply print commands 1 to i
	if (command_count<10){
		for(int i = 1; i <= command_count; i++){
		printf("%d\t", i);
		int l=0;
		while (history[i][l] != '\0'){
			printf("%c", history[i][l]);
			l++;
			}
		printf("\n");
		}
		return;
	}
	//if there has been more than 10 commands
	//print and wrap around using modulus
	int k = command_count + 1;
	for(int i = 0; i < 10; i++){
		printf("%d\t", k-10);
		int l=0;
		while (history[k % 10][l] != '\0'){
			printf("%c", history[k % 10][l]);
			l++;
		}
		printf("\n");
		k++;
	}
}


//retrieve command with command_number from history into tokens
void retrieve_from_history(char* buff, int command_number){
	for (int i=0; history[command_number%10][i] != '\0'; i++){
		buff[i] = history[command_number%10][i];
		printf("%c", buff[i]);
		buff[i+1] = '\0';
	}
	printf("\n");
	return;
}





/**
 * Read a command from the keyboard into the buffer 'buff' and tokenize it
 * such that 'tokens[i]' points into 'buff' to the i'th token in the command.
 * buff: Buffer allocated by the calling code. Must be at least
 *       COMMAND_LENGTH bytes long.
 * tokens[]: Array of character pointers which point into 'buff'. Must be at
 *       least NUM_TOKENS long. Will strip out up to one final '&' token.
 *       tokens will be NULL terminated (a NULL pointer indicates end of tokens).
 * in_background: pointer to a boolean variable. Set to true if user entered
 *       an & as their last token; otherwise set to false.
 */
void read_command(char *buff, char *tokens[], _Bool *in_background)
{
	*in_background = false;

	// Read input
	int length = read(STDIN_FILENO, buff, COMMAND_LENGTH-1);

	if(length < 0 && errno == EINTR){
		buff[0] = '\0';
	}

	if ( (length < 0) && (errno !=EINTR) ){
    	perror("Unable to read command. Terminating.\n");
    	exit(-1);  /* terminate with error */
	}


	// Null terminate and strip \n.
	buff[length] = '\0';
	if (buff[strlen(buff) - 1] == '\n') {
		buff[strlen(buff) - 1] = '\0';
	}


	// If the command is the internal command "!!", put into buff last command from history
	if (buff[0] == '!' && buff[1] == '!'){
		if (command_count == 0){
			write(STDOUT_FILENO, "SHELL: Unknown history command.\n", strlen("SHELL: Unknown history command.\n"));
			return;
			}
		retrieve_from_history(buff, command_count);
		}
		
	// If the command is the internal command "!"	
	else if (buff[0] == '!'){

		char command[length];
		for (int i = 1; i < length; i++){
			command[i-1] = buff[i];
		}
		//check if command number is valid
		int command_number = atoi(command);
		if (command_number == 0 || (abs(command_number-command_count) > 10) || (command_count == 0)){
			write(STDOUT_FILENO, "SHELL: Unknown history command.\n", strlen("SHELL: Unknown history command.\n"));
			return;
		}
		//if valid, retrieve command number from history
		else {
			retrieve_from_history(buff, command_number);
		}
	}


	add_history(buff);
	


	//write(STDOUT_FILENO, history[command_count], strlen(history[command_count]));
	//write(STDOUT_FILENO, "\n", strlen("\n"));

	// Tokenize (saving original command string)
	int token_count = tokenize_command(buff, tokens);

	if (token_count == 0) {
		return;
	}

	// Extract if running in background:
	if (token_count > 0 && strcmp(tokens[token_count - 1], "&") == 0) {
		*in_background = true;
		tokens[token_count - 1] = 0;
	}
}

void handle_SIGINT(){
	write(STDOUT_FILENO, "\n", strlen("\n"));
	print_history();
}


/**
 * Main and Execute Commands
 */
int main(int argc, char* argv[])
{
	char input_buffer[COMMAND_LENGTH];
	char *tokens[NUM_TOKENS];
	char currentdir[COMMAND_LENGTH];

	/* set up the signal handler */
	struct sigaction handler;
	handler.sa_handler = handle_SIGINT;
	handler.sa_flags = 0;
	sigemptyset(&handler.sa_mask);
	sigaction(SIGINT, &handler, NULL);

	while (true) {

		// Get command
		// Use write because we need to use read() to work with
		// signals, and read() is incompatible with printf().
		getcwd(currentdir, sizeof(currentdir));
		write(STDOUT_FILENO, currentdir, strlen(currentdir));
		write(STDOUT_FILENO, "$ ", strlen("$ "));
		_Bool in_background = false;
		read_command(input_buffer, tokens, &in_background);



		//Execution of shell commands

		 for (int i = 0; tokens[i] != NULL; i++){

		 	/////////////////////
		 	//INTERNAL COMMANDS//
		 	/////////////////////


		 	//exit
		 	if (strcmp(tokens[i],"exit") == 0){
		 		exit(0);
		 	}

		 	//pwd
		 	else if (strcmp(tokens[i],"pwd") == 0){
		 		getcwd(currentdir, sizeof(currentdir));
				write(STDOUT_FILENO, currentdir, strlen(currentdir));
		 		write(STDOUT_FILENO, "\n", strlen("\n"));
		 		tokens[i+1] = NULL;
		 		}


		 	//cd
		 	else if (strcmp(tokens[i],"cd") == 0){
		 		int success = chdir(tokens[i+1]);
		 		if (success < 0){
		 			write(STDOUT_FILENO, "Invalid directory.\n", strlen("Invalid directory.\n"));
		 		}
		 	}


		 	//history
		 	else if (strcmp(tokens[i],"history") == 0){
		 		print_history();
		 	}


		 	//type
		 	else if (strcmp(tokens[i], "type") == 0){
		 		if ((strcmp(tokens[i+1],"exit") == 0) || (strcmp(tokens[i+1],"pwd") == 0) || (strcmp(tokens[i+1],"cd") == 0)){
		 			write(STDOUT_FILENO, tokens[i+1], strlen(tokens[i+1]));
		 			write(STDOUT_FILENO, " is a shell300 builtin\n", strlen(" is a shell300 builtin\n"));
		 		}
		 		else {
		 			write(STDOUT_FILENO, tokens[i+1], strlen(tokens[i+1]));
		 			write(STDOUT_FILENO, " is external to shell300\n", strlen(" is external to shell300\n"));	
		 		}
		 	}



		 	/////////////////////
		 	//EXTERNAL COMMANDS//
		 	/////////////////////

		 	else {
		 		pid_t cpid;
		 		cpid = fork();
		 		if (cpid < 0){
		 			write(STDOUT_FILENO, "Error: fork() failed \n",strlen("Error: fork() failed \n"));
		 		}
		 		else if (cpid == 0){
		 			int success=execvp(tokens[0], tokens);
		 			if (success<0){
		 				write(STDOUT_FILENO, tokens[i], strlen(tokens[i]));
						write(STDOUT_FILENO, ": Unknown command.\n", strlen(": Unknown command.\n"));
		 			}
		 		}
		 		else if (in_background == false){
		 			waitpid(cpid, NULL, 0);
		 	}
		 	}
		 	while (waitpid(-1, NULL, WNOHANG) > 0){} //Zombie cleanup
		 	break; //break to ignore nonrelevant arguments
		 }
	}
	return 0;
}