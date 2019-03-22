// Shell starter file
// You may make any changes to any part of this file.

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define COMMAND_LENGTH 1024
#define NUM_TOKENS (COMMAND_LENGTH / 2 + 1)
#define HISTORY_ARRAY_LENGTH 10
#define HISTORY_DEPTH 10
char history[HISTORY_DEPTH][COMMAND_LENGTH];
//char* numberOfHistoryCounter[HISTORY_ARRAY_LENGTH];
int historyCounter;
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

	if (length < 0) {
		perror("Unable to read command from keyboard. Terminating.\n");
		exit(-1);
	}

	// Null terminate and strip \n.
	buff[length] = '\0';
	if (buff[strlen(buff) - 1] == '\n') {
		buff[strlen(buff) - 1] = '\0';
	}

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

 /*  int i=count;
    int j=0;
    int k=0;

    // len = sizeof(tokens[0]);
    if (strcmp(tokens[0],"")!=0){
       if (strlen(tokens[0])!=0){


          memset(history[i], 0, strlen(history[i]));
          for(j=0; j<COMMAND_LENGTH; j++){
             if(i>9) i=i-10;
             if (tokens[0][j] == '\0'){
                history[i][j] = ' ';
                k++;
             }
             else
               history[i][j] = tokens[0][j];

            if(tokens[k] == NULL){
               break;
*/
void collectHistory(char* tokens[])
{
	int cols = historyCounter;
	int rows = 0;
	//int index = 0;
	if (historyCounter < 10)
	{
		//index = historyCounter;
		if(strcmp(tokens[0], "\0") != 0)
		{
			if(strlen(tokens[0]) != 0)
			{
				while(rows < COMMAND_LENGTH)
				{
					if (tokens[0][rows] == '\0')
					{ 
						history[cols][rows] = ' ';
					}
	             	else
	             	{
	             		history[cols][rows] = tokens[0][rows];
	             	}
					rows++;
					//rows = (rows + 1) % HISTORY_ARRAY_LENGTH;
				}
				//*numberOfHistoryCounter[index] = historyCounter;
			}
		}
	}
	else
	{
		cols = historyCounter - 10;
		//cols = historyCounter % HISTORY_ARRAY_LENGTH;
		//index = historyCounter % HISTORY_ARRAY_LENGTH;
		if (strcmp(tokens[0], "\0") != 0)
		{
			if (strlen(tokens[0]) != 0)
			{
				while(rows < COMMAND_LENGTH)
				{
					if (tokens[0][rows] == '\0')
					{ 
						history[cols][rows] = ' ';
					}
	             	else
	             	{
	             		history[cols][rows] = tokens[0][rows];
	             	}
					rows++;
				}
			}
			//*numberOfHistoryCounter[index] = historyCounter;
		}
	}
	historyCounter++;
}

void displayHistory()
{
	char index[] = " ";
	//int counter = historyCounter;
	for (int i = 0; i < HISTORY_ARRAY_LENGTH; i++)
	{
		sprintf(index, "%d",historyCounter);
		write(STDOUT_FILENO, index, strlen(index));
		write(STDOUT_FILENO, "\t", strlen("\t"));
		write(STDOUT_FILENO, history[i], sizeof(history[i]));
		write(STDOUT_FILENO, "\n", strlen("\n"));
	}
}

/**
 * Main and Execute Commands
 */
int main(int argc, char* argv[])
{
	char input_buffer[COMMAND_LENGTH];
	char *tokens[NUM_TOKENS];
	pid_t cpid;
	int currentStatus;
	char currentPath[COMMAND_LENGTH];

	while (true) {

		// Get command
		// Use write because we need to use read() to work with
		// signals, and read() is incompatible with printf().
		getcwd(currentPath, sizeof(currentPath));
		write(STDOUT_FILENO, currentPath, strlen(currentPath));
		write(STDOUT_FILENO, "$ ", strlen("$ "));
		_Bool in_background = false;
		read_command(input_buffer, tokens, &in_background);
		//compare the command and output external or buildin
		if (tokens[0] != NULL)
		{
			if (strcmp(tokens[0], "exit") == 0)
			{
				exit(0);
			}
			else if (strcmp(tokens[0], "pwd") == 0)
			{
				//getcwd(currentPath, sizeof(currentPath));
				write(STDOUT_FILENO, currentPath, strlen(currentPath));
				currentStatus = 1;
			}
			else if (strcmp(tokens[0], "cd") == 0)
			{
				//print out error if there is no such directory
				int dirError = chdir(tokens[1]);
				if (dirError == -1)
				{
					write(STDOUT_FILENO, "Invalid directory.\n", strlen("No such directory.\n"));
				}
				currentStatus = 1;
			}
			else if (strcmp(tokens[0], "type") == 0)
			{
				char outputBuiltin[COMMAND_LENGTH];
				char outputExternal[COMMAND_LENGTH];
				sprintf(outputBuiltin, "%s is a shell300 builtin\n", tokens[1]);
				sprintf(outputExternal, "%s is external to shell300\n", tokens[1]);
				if (strcmp(tokens[1], "cd") == 0 || strcmp(tokens[1], "pwd") == 0 || strcmp(tokens[1], "exit") == 0 || strcmp(tokens[1], "type") == 0)
				{
					write(STDOUT_FILENO, outputBuiltin, strlen(outputBuiltin));
					currentStatus = 1;
				}
				else
				{
					write(STDOUT_FILENO, outputExternal, strlen(outputExternal));
					currentStatus = 1;
				}
			}
			else if (strcmp(tokens[0], "type") == 0 && (tokens[1] == NULL))
			{
				char missingSecondCommand[COMMAND_LENGTH];
				sprintf(missingSecondCommand, "Missing second command.\n");
				write(STDOUT_FILENO, missingSecondCommand, strlen(missingSecondCommand));
			}
			else if (strcmp(tokens[0], "history") == 0)
			{
				displayHistory();
			}
			else if (strcmp(tokens[0], "!!") != 0)
			{
				if (strcmp(tokens[0], "!n") != 0)
				{
					collectHistory(tokens);
					currentStatus = 1;
				}
			}
			//how to compare with newline or enter to output nothing
			else if (strcmp(tokens[0], "\n") == 0)
			{
				//getcwd(currentPath, sizeof(currentPath));
				write(STDOUT_FILENO, currentPath, strlen(currentPath));
				write(STDOUT_FILENO, "$", strlen("$"));
				currentStatus = 1;
			}
		}

		// DEBUG: Dump out arguments:
		/*for (int i = 0; tokens[i] != NULL; i++) {
			write(STDOUT_FILENO, "   Token: ", strlen("   Token: "));
			write(STDOUT_FILENO, tokens[i], strlen(tokens[i]));
			write(STDOUT_FILENO, "\n", strlen("\n"));
		}*/
		if (in_background) {
			write(STDOUT_FILENO, "Run in background.", strlen("Run in background."));
		}

		/**
		 * Steps For Basic Shell:
		 * 1. Fork a child process
		 * 2. Child process invokes execvp() using results in token array.
		 * 3. If in_background is false, parent waits for
		 *    child to finish. Otherwise, parent loops back to
		 *    read_command() again immediately.
		 */

		if (currentStatus == 0)
		{
			cpid = fork();
			if (cpid == 0)
			{
				int error = execvp(tokens[0], tokens);
				if (error == -1)
				{
					char unknownCommand[COMMAND_LENGTH];
					sprintf(unknownCommand, "%s: Unknown command.\n", tokens[0]);
					write(STDOUT_FILENO, unknownCommand, strlen(unknownCommand));
					//should this local outside of the if or not?
					exit(0);
				}
				//exit(0);
			}
			else if (cpid < 0)
			{
				write(STDOUT_FILENO, "error with forking\n", strlen("error with forking\n"));
				exit(-1);
			}
			else
			{
				if (in_background == false)
				{
					while (waitpid(-1, NULL, WNOHANG) > 0)
						;
					waitpid(cpid, &currentStatus, 0);
				}
				
			}
		}
	}
	return 0;
}


//give type another if to show the error when just input type
//something wrong with fork, can not show the second line
//when input enter, should show differnt thing?