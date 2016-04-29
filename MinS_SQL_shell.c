#include "MinS_SQL_shell.h"

#define CMD_SIZE 1024

// ** For fetching command ** //
void fetch_cmd()
{
	bool cmd_continue = true;
	char *cmd;
	//memset(cmd,'\0',CMD_SIZE);
	
	while(1)
	{
		// Fetch command
		cmd = read_cmd();
		// For debugging 	
		printf("String is %s\n",cmd);
		// Parse the command
		parse_cmd(cmd);
		// Clear the command
		memset(cmd,'\0',CMD_SIZE);
	}
}

// For read-in command
char* read_cmd()
{
	char c;
	int index = 0;
	char *buffer = malloc(CMD_SIZE*sizeof(char));
	/* Loop for getchar */
	while(1){
		c = getchar();
		if(c == '\n' || c == EOF){
			buffer[index] = '\0';
			return buffer;	
		}
		else{
			buffer[index] = c;
			index++;
		}
		// TODO: Check if > LINE_BUF
	}
}

// For parsing command
int parse_cmd(char *cmd)
{
	// Compare and do the correspond command - fetch one string by on string
	
}
