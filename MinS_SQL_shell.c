#include "MinS_SQL_shell.h"
#include "stringOperation.h"

#define CMD_SIZE 1024

typedef struct stringLink{
	char name[64];
	struct stringLink *next;
} strLink;

strLink *forSelect;

void AddStrLink(strLink *header , char *name);

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
		// printf("String is %s\n",cmd);
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
	char select_cmd[512];
	memset(pre_cmd,'\0',64*sizeof(char));
	memset(post_cmd,'\0',1024*sizeof(char));
	memset(select_cmd,'\0',512*sizeof(char));
	
	// The first header have empty name 
	forSelect = (strLink *) malloc(sizeof(strLink));
	forSelect->next = 0;
	
	while(1)
	{
		sscanf(cmd , "%s %[^\n]",pre_cmd,post_cmd);
		// change main control to post command
		if(post_cmd[0] == '\0')
		{
			// if post command have nothing 
			break;
		}
		// print out for debugging
		printf("Pre_Command is %s , Post_Command is %s\n",pre_cmd,post_cmd);	
	
		// TODO : do the correspond thing
		int cmp = strcmp_ctrl(pre_cmd,post_cmd,1);
		// printf("CMP: %d\n",cmp);
		
		// First , to compare with "SELECT"
		if(strcmp_ctrl(pre_cmd,"SELECT",0))
		{
			// if return 1 , compare !
			// Then Fetch the SELECT strings 
			sscanf(post_cmd , "%[^FROM] %s" , select_cmd , post_cmd);
			printf("%s , size : %d\n",select_cmd,(int)strlen(select_cmd));
			// After we have the strings after SELECT , we can manipulate it!
			while(1)
			{
				if(strlen(select_cmd) <= 2 && strcmp_ctrl(select_cmd,"*",0))
				{
					// Compare with "*" , notice : now's string length is 2
					AddStrLink(forSelect,"*");
					printf("Fetch * for select\n");
					break;
				}
				else
				{
					char cmpStr[32];
					char leftover[128];
					sscanf(select_cmd , "%[^,]%*[,]%[^\n]",cmpStr , leftover);
					int filter = 0 , filter2 = 0;
					// Check if leftover have "," in it
					for(int i =0; i < strlen(leftover) ;i++)
					{
						// Check if now have contained ","
						if(leftover[i] != ',')
						{
							filter = 0;
						}
						else
						{
							filter = 1;
							break;
						}
					}
					for(int i = 0; i < strlen(select_cmd) ; i++)
					{
						// Check if type is only one string
						if(select_cmd[i] != ',')
						{
							filter2 = 0;
						}
						else
						{
							filter2 = 1;
							break;
						}
					}
					if(filter == 0)
					{
						// Don't have the string , now the leftover is the last string we want
						if(filter2 == 0)
						{
							AddStrLink(forSelect,cmpStr);
							break;
						}
						else
						{
							AddStrLink(forSelect,cmpStr);
							AddStrLink(forSelect,leftover);
							break;
						}	
					}
					else
					{
						// We have to store cmpStr as the attribute we want! , store cmpStr as 
						AddStrLink(forSelect,cmpStr);
						memset(select_cmd , '\0' , strlen(select_cmd)*sizeof(char));
						strcpy(select_cmd , leftover);
						printf("%s \n",select_cmd);
					}
				}
			} // <SELECT> while Loop end , and get the SELECT string token
			
			
		}
		else
		{
			// get the string between SQL command
			// TODO : make another shell command here
			printf("Error input");
		}
		// clear cmd 
		memset(cmd,'\0',strlen(cmd)*sizeof(char));
		// map post cmd to cmd
		strcpy(cmd , post_cmd);
		// clear the parse string
		memset(pre_cmd,'\0',64*sizeof(char));	
		memset(post_cmd,'\0',1024*sizeof(char));
		memset(select_cmd,'\0',512*sizeof(char));
	}
	// Clean string buffer
	memset(pre_cmd,'\0',64*sizeof(char));
	memset(post_cmd,'\0',1024*sizeof(char));
	
	// Print for debug 
	strLink *header;
	header = forSelect;
	printf("Selection:");
	while(header->next != NULL)
	{
		printf("%s ",header->next->name);
		header = header->next;
	}
	printf("\n");
}

void AddStrLink(strLink *header , char *name)
{
	strLink *link;
	link = header;
	while(link->next != NULL)
	{
		link = link->next;
	}
	strLink *temp = (strLink *)malloc(sizeof(strLink));
	temp->next = NULL;
	memset(temp->name , '\0' , 64);
	strcpy(temp->name , name);
	link->next = temp;
}
