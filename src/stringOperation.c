#include "stringOperation.h"

int strcmp_ctrl(char *str1 , char *str2,int mode)
{
	int len1 = strlen(str1);
	int len2 = strlen(str2);
	char *temp1 = malloc(sizeof(char)*len1);
	char *temp2 = malloc(sizeof(char)*len2);
	strcpy(temp1,str1);
	strcpy(temp2,str2);
	
	if(len1 != len2)
	{
		// two length are different
		free(temp1);
		free(temp2);
		return 0;
	}
	else
	{
		// same length 
		for(int i = 0; i < len1 ; i++)
		{
			// If user want to ignore case
			if(mode == 1)
			{
				// string with ignore-case character compare ; all turn into up-case
				if(temp1[i] >= 97 && temp1[i] <= 122)
				{
					// down case 
					temp1[i] -= 32;
				}
				if(temp2[i] >= 97 && temp2[i] <= 122)
				{
					// down case 
					temp2[i] -= 32;
				}
			}
			
			if(temp1[i] != temp2[i])
			{
				// when one compare fail , return -1
				free(temp1);
				free(temp2);
				return 0;
			}
		}
		free(temp1);
		free(temp2);
		return 1;
	}
}
