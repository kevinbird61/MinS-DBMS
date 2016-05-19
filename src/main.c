#include <stdio.h>
#include "MinS_SQL_shell.h"

#define READLEN 100
#define FILEBOOK "../data/books.txt"
#define FILESELL "../data/sellRecord.txt"

typedef struct book{
	char isbn[32];
	char author[32];
	char title[64];
	char price[16];
	char subject[16];
	struct book *next;
} books;

typedef struct sell{
	char uid[16];
	char no[16];
	char isbn_no[32];
	struct sell *next;
} sells;

typedef struct attrivalue{
    char value[64];
    struct attrivalue * next;
	books * booksInfo;
	sells * sellsInfo;    
} attri_Value;

char **attri_Name;
attri_Value **bucket_Header;
books *books_Header;
sells *sells_Header;

void read_SrcFile();
void addBucket(char *attri , books *b_info , sells *s_info, int attri_number); // TODO : add 2 type of info input , when attri_number between 0~4 belong to books ; between 5~7 belong to sells 
books *addBooks(books *header , char *is , char *au,char *tit,char *pri,char *sub); // TODO : need to return the current books pointer
sells *addSells(sells *header , char *ui,char *n,char *is_n); // TODO : need to return the current sells pointer
void printBucket();
int hash33(char *key);

int main(int argc , char *argv[])
{
	// Using C to Implement MySQL
	printf("Welcome to Use Min's DBMS , implemented in C ,\n");
	printf("which programmed by Kevin Chiu\n");
	// Read in the resource file
	read_SrcFile();
	// Fetch command , and do the correspond thing 
	while(1)
	{
		int print = fetch_cmd();
		// Print for debug 
		/*
		if(print)
		{
			strLink *header;
			header = forSelect;
			printf("SELECT :");
			while(header->next != NULL)
			{
				printf(",%s ",header->next->name);
				header = header->next;
			}
			printf("\n");
	
			header = forFrom;
			printf("FROM :");
			while(header->next != NULL)
			{
				printf(",%s ",header->next->name);
				header = header->next;
			}
			printf("\n");

			header = forWhere;
			printf("WHERE :");
			while(header->next != NULL)
			{
				printf(",%s ",header->next->name);
				header = header->next;
			}
			printf("\n");
		}
		*/
		printBucket();
		// And Now we have storage data and SQL query
		
	}
	
	return 0;
}

void read_SrcFile()
{
	// For variable defined
    bucket_Header = (attri_Value** )malloc(8*sizeof(attri_Value*));
    char **attri_v = (char** ) malloc(8*sizeof(char*));
	attri_Name = (char** ) malloc(8*sizeof(char*));
	books_Header = malloc(sizeof(books));
	sells_Header = malloc(sizeof(sells));
	// Initialized
	for(int i = 0; i < 8 ; i++)
	{
	    attri_Name[i] = (char* ) malloc(32*sizeof(char));
	    attri_v[i] = (char* ) malloc(32*sizeof(char));
	    memset(attri_v[i] , '\0' , 32);
	    bucket_Header[i] = (attri_Value*) malloc(10*sizeof(attri_Value));
	    for(int j = 0; j < 10; j++)
	    {
	        bucket_Header[i][j].next = NULL;
	        //bucket_Header[i][j].booksInfo = NULL;
	        //bucket_Header[i][j].sellsInfo = NULL;
	    }
	}
	// Read the resource data
	FILE *fp;
	fp = fopen(FILEBOOK,"r");
	// Store for table name
	char table_name_1[5];
	strcpy(table_name_1 , "books");
	char *str = malloc(sizeof(char)*READLEN);
	int count = 0;
	while(fgets(str , sizeof(char)*READLEN , fp) != NULL)
	{
	    // Fetch first line to build the attributes names
	    if(count == 0)
	    {
	        // Parse the Attributed name
	        sscanf(str,"%*s %[^|]|%[^|]|%[^|]|%[^|]|%[^ ] %*s",attri_Name[0],attri_Name[1],attri_Name[2],attri_Name[3],attri_Name[4]);
	        // And Now we can do the Main thing
	        count = 1;
	    }
	    else
	    {
	        // Dealing with the Main String
	        sscanf(str,"%[^|]|%[^|]|%[^|]|%[^|]|%s",attri_v[0],attri_v[1],attri_v[2],attri_v[3],attri_v[4]);
	        books *data = addBooks(books_Header,attri_v[0],attri_v[1],attri_v[2],attri_v[3],attri_v[4]);
	        // Using Hash33 to get the attributes
	        for(int i = 0 ; i < 5; i++)
	        {
	            addBucket(attri_v[i] , data , NULL , i);
	            memset(attri_v[i] , '\0' , 32);
	        }
	    }
	}
	// Read for the next resource
	FILE *fp2;
	fp2 = fopen(FILESELL,"r");
	// Store for table name
	char table_name_2[10];
	strcpy(table_name_2 , "sellRecord");
	memset(str ,'\0' , READLEN*sizeof(char));
	count = 0;
	while(fgets(str , sizeof(char)*READLEN , fp2) != NULL )
	{
	    // Filt out the blank line
	    if(strlen(str)<=3)
		{
			break;
		}
		// Fetch the first line
	    if(count == 0)
	    {
	        // Parse the Attributed name
	        sscanf(str,"%*s %[^|]|%[^|]|%[^ ] %*s",attri_Name[5],attri_Name[6],attri_Name[7]);
	        // And Now we can do the Main thing
	        count = 1;
	    }
	    else
	    {
	        // Dealing with the Main String
	        sscanf(str,"%[^|]|%[^|]|%s",attri_v[5],attri_v[6],attri_v[7]);
	        sells *data = addSells(sells_Header,attri_v[5],attri_v[6],attri_v[7]);
	        // Using Hash33 to get the attributes
	        for(int i = 5 ; i < 8; i++)
	        {
	            addBucket(attri_v[i] ,NULL,data, i);
	            memset(attri_v[i] , '\0' , 32);
	        }
	    }
	}
    // Now we store back into the file
	// And then Create the file for these attributes
	for(int i = 0 ;i < 8 ; i++)
	{
		FILE *fwrite;
		char filename[64];
		memset(filename ,'\0', 64*sizeof(char));
		strcat(filename,"table/");
		strcat(filename,attri_Name[i]);
		strcat(filename,".txt");
		fwrite = fopen(filename,"w+");
		// Now do the file wrote in 
		char strin[1024];
		memset(strin,'\0',1024*sizeof(char));
		for(int j = 0 ; j < 10 ; j++)
		{
			sprintf(strin,"Buckets[%d]",j);
			attri_Value *header = &bucket_Header[i][j];
            while(header->next != NULL)
            {
            	strcat(strin,",");
            	strcat(strin,header->next->value);
                header = header->next;
            }
            // And now we have bucket that can wrote into file
            fprintf(fwrite,"%s\n",strin);
            memset(strin,'\0',1024*sizeof(char));
		}
		fclose(fwrite);
		memset(filename,'\0',64*sizeof(char));
	}
	
	// For struct debugging
	books *header;
	header = books_Header;
	printf("Books Header:\n");
	// First header is empty
	while(header->next != NULL)
	{
		printf(",%s,%s,%s,%s,%s \n",header->next->isbn,header->next->author,header->next->title,header->next->price,header->next->subject);	
		header = header->next;
	}

	sells *header2 = sells_Header;
	printf("Sells Header:\n");
	// First header is empty
	while(header2->next != NULL)
	{
		printf(",%s,%s,%s \n",header2->next->uid,header2->next->no,header2->next->isbn_no);	
		header2 = header2->next;
	}
	fclose(fp);
	fclose(fp2);
}

sells *addSells(sells *header , char *ui,char *n,char *is_n)
{
	while(header->next)
	{
		header = header->next;
	}
	sells *temp = (sells *) malloc(sizeof(sells));
	strcpy(temp->uid , ui);
	strcpy(temp->no , n);
	strcpy(temp->isbn_no , is_n);
	temp->next = NULL;
	header->next = temp;
	temp = NULL;
	free(temp);
	return header->next;
}

books *addBooks(books *header , char *is , char *au,char *tit,char *pri,char *sub)
{
	while(header->next != NULL)
	{
		header = header->next;
	}
	books *temp = (books *) malloc(sizeof(books));
	strcpy(temp->isbn,is);
	strcpy(temp->author,au);
	strcpy(temp->title,tit);
	strcpy(temp->price,pri);
	strcpy(temp->subject,sub);
	temp->next = NULL;
	header->next = temp;
	temp = NULL;
	free(temp);
	return header->next;
}

void addBucket(char *attri,books *b_info , sells *s_info,int attri_number)
{
    attri_Value *temp = (attri_Value *) malloc(sizeof(attri_Value));
    strcpy(temp->value , attri);
    temp->next = NULL;
    
    attri_Value *header;
    header = &bucket_Header[attri_number][hash33(attri)];
    while(1)
    {
        if(header->next != NULL)
        {
            header = header->next;   
        }
        else
        {
            header->next = temp;
            header->next->booksInfo = b_info;
            header->next->sellsInfo = s_info;
            break;
        }
    }
}

void printBucket()
{
    for(int i = 0; i < 8 ; i++)
    {
        for(int j = 0; j < 10 ; j++)
        {
            attri_Value *header = &bucket_Header[i][j];
            printf("For Attributes: %s , Buckets[%d] :" ,attri_Name[i], j);
            while(header->next != NULL)
            {
                printf("-> %s",header->next->value);
                //printf(" and %s ", header->next->booksInfo->isbn);
                //printf(" and %s ", header->next->sellsInfo->uid);
                header = header->next;
            }
            printf("\n");
        }
    }   
}

int hash33(char *key)
{
    int i, k;
	unsigned int hv = 0;
	k=strlen(key);
	for(i=0;i<k;i++){
		hv=(hv<<5)+hv+key[i];
	}
	hv=hv%10;
	return hv;
}
