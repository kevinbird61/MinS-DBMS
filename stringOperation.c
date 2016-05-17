#include "stringOperation.h"

int strcmp_ctrl(char *str1 , char *str2,int mode)
{
	int len1 = strlen(str1);
	int len2 = strlen(str2);
	//printf("str1 : %d ; str2 : %d\n",a,b);
	if(len1 != len2)
	{
		// two length are different
		return -1;
	}
	else
	{
		// same length 
		int i;
		for(i = 0; i < len1 ; i++)
		{
			// If user want to ignore case
			if(mode == 1)
			{
				// string with ignore-case character compare ; all turn into up-case
				if(str1[i] >= 97 && str1[i] <= 122)
				{
					// down case 
					str1[i] -= 32;
				}
				if(str2[i] >= 97 && str2[i] <= 122)
				{
					// down case 
					str2[i] -= 32;
				}
			}
			
			if(str1[i] != str2[i])
			{
				// when one compare fail , return -1
				return -1;
			}
		}
	}
	return 1;
}
