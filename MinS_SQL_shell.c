#include "MinS_SQL_shell.h"
#include "stringOperation.h"

#define CMD_SIZE 1024

// ** For fetching command ** //
void fetch_cmd()
{
	bool cmd_continue = true;
	char *cmd;
	
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
	char pre_cmd[64];
	char post_cmd[1024];
	while(1)
	{
		sscanf(cmd , "%s %[^\n]",pre_cmd,post_cmd);
		// print out for debugging
		printf("Pre_Command is %s , Post_Command is %s\n",pre_cmd,post_cmd);	
	
		// TODO : do the correspond thing
		int cmp = strcmp_ctrl(pre_cmd,post_cmd,1);
		printf("CMP: %d\n",cmp);
		
		// First , to compare with "SELECT"
		if(strcmp_ctrl(pre_cmd,"SELECT",0))
		{
			// if return 1 , compare !
			
		}
		else if(strcmp_ctrl(pre_cmd,"FROM",0))
		{
			// if return 1 , compare !
			
		}
		else if(strcmp_ctrl(pre_cmd,"WHERE",0))
		{
			// if return 1 , compare
			
		}
		// For addition function
		else if(strcmp_ctrl(pre_cmd,"ADD",0))
		{
			// Add the data base , and build the table
		}
		else
		{
			// get the string between SQL command
			
		}
		
		// change main control to post command
		if(post_cmd[0] == '\0')
		{
			// if post command have nothing 
			break;
		}
		// clear cmd 
		memset(cmd,'\0',strlen(cmd));
		// map post cmd to cmd
		strcpy(cmd , post_cmd);
		// clear the parse string
		memset(pre_cmd,'\0',64);	
		memset(post_cmd,'\0',1024);
	}
	// Clean string buffer
	memset(pre_cmd,'\0',64);
	memset(post_cmd,'\0',1024);
}
