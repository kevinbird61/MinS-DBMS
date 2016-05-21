#include <stdio.h>
#include "MinS_SQL_shell.h"
#include "stringOperation.h"

#define READLEN 100
#define FILEBOOK "../data/books.txt"
#define FILESELL "../data/sellRecord.txt"
#define PDEBUG false

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
char table_name_1[5];
char table_name_2[10];
attri_Value **bucket_Header;
books *books_Header;
sells *sells_Header;

void read_SrcFile();
void Do_SQL();
int judgeSelect(strLink *header,int mode);
void addBucket(char *attri , books *b_info , sells *s_info, int attri_number); 
books *addBooks(books *header , char *is , char *au,char *tit,char *pri,char *sub); 
sells *addSells(sells *header , char *ui,char *n,char *is_n);
void printBucket();
void printSQL();
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
		int result = fetch_cmd();
		// Print for debug 
		if(result==1)
		{
			printSQL();	
			if(PDEBUG)
			{
				printBucket();
			}
		}
		else if(result==-1)
		{
			break;
		}
		// And Now we have storage data and SQL query
		Do_SQL();
		
		printf("==============================================\n");
	}
	
	return 0;
}

void Do_SQL()
{	
	// Check for the FROM clause : FETCH for join and on
	strLink *header; 
	header = forFrom;
	while(header->next != NULL)
	{
		char comp1[64];
		char comp2[64];
		char comp1_attr[64];
		char comp2_attr[64];
		memset(comp1,'\0',64*sizeof(char));
		memset(comp2,'\0',64*sizeof(char));
		memset(comp1_attr,'\0',64*sizeof(char));
		memset(comp2_attr,'\0',64*sizeof(char));
		sscanf(header->next->name,"%[^/^JOIN/]%*[/JOIN$/]%[^/^ON/]%*[/ON$/]%[^/^=/]%*[=]%s",comp1,comp2,comp1_attr,comp2_attr);
		printf("Cut for FORM : %s %s %s %s\n",comp1,comp2,comp1_attr,comp2_attr);
		sscanf(comp1,"%s",comp1);
		sscanf(comp2,"%s",comp2);
		// Do for more robust string compare , and Check if comp1 and comp2 is in your table name,
		// and implement with it (rebuild the tables).
		if(strcmp_ctrl(comp1,table_name_1,0))
		{
			// comp1 = table1
			if(strcmp_ctrl(comp2,table_name_2,0))
			{
				// comp2 = table2 , do the table attribute match
				// TODO : when 2 table all get , need to check the join string 
				
				printf("Match!\n");
			}
			else
			{
				// Consider when comp2 empty
				if(strlen(comp2)<=1)
				{
					// Only need to consider comp1 , now get the SELECT strings 
					// Check for the Select clause : legal or not
					strLink *header2;
					header2 = forSelect;
					int count = 0;
					printf("Judge SELECT :");
					while(header2->next != NULL)
					{
						if(header2->next->name[0]=='*' && count == 0)
						{
							// If compare with this , check whether the other behind
							if(strlen(header2->next->name)>2)
							{
								printf("Error with string behind '*' , len is %d\n",(int)strlen(header2->next->name));
								return;
							}
							else
							{
								// Prepare the print out all the attributes of that table_1 !
								printf("List all attribute in table - %s :\n",table_name_1);
								printf("%-20s %-20s %-20s %-20s %-20s\n",attri_Name[0],attri_Name[1],attri_Name[2],attri_Name[3],attri_Name[4]);
								for(int j=0;j<10;j++)
								{
									attri_Value *header = &bucket_Header[0][j];
									while(header->next != NULL)
									{
										// Check for the WHERE clause : FETCH for GROUP and "=" for compare
										strLink *header_w = forWhere;
										while(header_w->next != NULL)
										{
											char comp_1[64];
											memset(comp_1,'\0',64*sizeof(char));
											char comp_2[64];
											memset(comp_2,'\0',64*sizeof(char));
											sscanf(header_w->next->name,"%[^=]%*[=]%[^\n]",comp_1,comp_2);
											sscanf(comp_1,"%s",comp_1);
											
											printf("comp1: %s , comp2: %s\n",comp_1,comp_2);
											
											if(strcmp_ctrl(comp_1,attri_Name[0],0))
											{
												if(strcmp_ctrl(header->next->booksInfo->isbn,comp_2,0))
												{
													// Success and print 
													printf("%-20s ", header->next->booksInfo->isbn);
													printf("%-20s ", header->next->booksInfo->author);
													printf("%-20s ", header->next->booksInfo->title);
													printf("%-20s ", header->next->booksInfo->price);
													printf("%-20s \n", header->next->booksInfo->subject);			
												}
											}
											else if(strcmp_ctrl(comp_1,attri_Name[1],0))
											{
												if(strcmp_ctrl(header->next->booksInfo->author,comp_2,0))
												{
													// Success and print 
													printf("%-20s ", header->next->booksInfo->isbn);
													printf("%-20s ", header->next->booksInfo->author);
													printf("%-20s ", header->next->booksInfo->title);
													printf("%-20s ", header->next->booksInfo->price);
													printf("%-20s \n", header->next->booksInfo->subject);			
												}
											}
											else if(strcmp_ctrl(comp_1,attri_Name[2],0))
											{
												if(strcmp_ctrl(header->next->booksInfo->title,comp_2,0))
												{
													// Success and print 
													printf("%-20s ", header->next->booksInfo->isbn);
													printf("%-20s ", header->next->booksInfo->author);
													printf("%-20s ", header->next->booksInfo->title);
													printf("%-20s ", header->next->booksInfo->price);
													printf("%-20s \n", header->next->booksInfo->subject);			
												}
											}
											else if(strcmp_ctrl(comp_1,attri_Name[3],0))
											{
												if(strcmp_ctrl(header->next->booksInfo->price,comp_2,0))
												{
													// Success and print 
													printf("%-20s ", header->next->booksInfo->isbn);
													printf("%-20s ", header->next->booksInfo->author);
													printf("%-20s ", header->next->booksInfo->title);
													printf("%-20s ", header->next->booksInfo->price);
													printf("%-20s \n", header->next->booksInfo->subject);			
												}
											}
											else if(strcmp_ctrl(comp_1,attri_Name[4],0))
											{
												if(strcmp_ctrl(header->next->booksInfo->subject,comp_2,0))
												{
													// Success and print 
													printf("%-20s ", header->next->booksInfo->isbn);
													printf("%-20s ", header->next->booksInfo->author);
													printf("%-20s ", header->next->booksInfo->title);
													printf("%-20s ", header->next->booksInfo->price);
													printf("%-20s \n", header->next->booksInfo->subject);			
												}
											}
											else
											{
												if(strlen(comp_1) == 0)
												{
													printf("%-20s ", header->next->booksInfo->isbn);
													printf("%-20s ", header->next->booksInfo->author);
													printf("%-20s ", header->next->booksInfo->title);
													printf("%-20s ", header->next->booksInfo->price);
													printf("%-20s \n", header->next->booksInfo->subject);		
												}
												else
												{
													printf("Not found this attribute-%s in table %s\n",comp_1,table_name_1);	
												}
											}
											header_w = header_w->next;
										}
										header = header->next;
									}
								}
								
								break;
							}
						}
						else
						{
							// TODO : implement selected attri_Name , and also need to check WHERE
							// Judge whether if attri_Name SELECT clause have
							int result = judgeSelect(forSelect,0);
							printf("%d\n",result);
							if((result & 1)==1)
								printf("%-20s ",attri_Name[0]);
							if((result & 2)==2)
								printf("%-20s ",attri_Name[1]);
							if((result & 4)==4)
								printf("%-20s ",attri_Name[2]);
							if((result & 8)==8)
								printf("%-20s ",attri_Name[3]);
							if((result & 16)==16)
								printf("%-20s ",attri_Name[4]);
							printf("\n");	
							
							for(int j=0;j<10;j++)
								{
									attri_Value *header = &bucket_Header[0][j];
									while(header->next != NULL)
									{
										// Check for the WHERE clause : FETCH for GROUP and "=" for compare
										strLink *header_w = forWhere;
										while(header_w->next != NULL)
										{
											char comp_1[64];
											memset(comp_1,'\0',64*sizeof(char));
											char comp_2[64];
											memset(comp_2,'\0',64*sizeof(char));
											//sscanf(header_w->next->name,"%[^\n]", comp_1);
											sscanf(header_w->next->name,"%[^=] = %[^\n]",comp_1,comp_2);
											sscanf(comp_1,"%s",comp_1);
											
											//printf("comp1: %s , comp2: %s\n",comp_1,comp_2);
											
											if(strcmp_ctrl(comp_1,attri_Name[0],0))
											{
												if(strcmp_ctrl(header->next->booksInfo->isbn,comp_2,0))
												{
													// Success and print 
													if((result & 1)==1)
														printf("%-20s ", header->next->booksInfo->isbn);
													if((result & 2)==2)
														printf("%-20s ", header->next->booksInfo->author);
													if((result & 4)==4)
														printf("%-20s ", header->next->booksInfo->title);
													if((result & 8)==8)
														printf("%-20s ", header->next->booksInfo->price);
													if((result & 16)==16)
														printf("%-20s ", header->next->booksInfo->subject);
													printf("\n");		
												}
											}
											else if(strcmp_ctrl(comp_1,attri_Name[1],0))
											{
												if(strcmp_ctrl(header->next->booksInfo->author,comp_2,0))
												{
													// Success and print 
													if((result & 1)==1)
														printf("%-20s ", header->next->booksInfo->isbn);
													if((result & 2)==2)
														printf("%-20s ", header->next->booksInfo->author);
													if((result & 4)==4)
														printf("%-20s ", header->next->booksInfo->title);
													if((result & 8)==8)
														printf("%-20s ", header->next->booksInfo->price);
													if((result & 16)==16)
														printf("%-20s ", header->next->booksInfo->subject);
													printf("\n");			
												}
											}
											else if(strcmp_ctrl(comp_1,attri_Name[2],0))
											{
												if(strcmp_ctrl(header->next->booksInfo->title,comp_2,0))
												{
													// Success and print 
													if((result & 1)==1)
														printf("%-20s ", header->next->booksInfo->isbn);
													if((result & 2)==2)
														printf("%-20s ", header->next->booksInfo->author);
													if((result & 4)==4)
														printf("%-20s ", header->next->booksInfo->title);
													if((result & 8)==8)
														printf("%-20s ", header->next->booksInfo->price);
													if((result & 16)==16)
														printf("%-20s ", header->next->booksInfo->subject);
													printf("\n");			
												}
											}
											else if(strcmp_ctrl(comp_1,attri_Name[3],0))
											{
												if(strcmp_ctrl(header->next->booksInfo->price,comp_2,0))
												{
													// Success and print 
													if((result & 1)==1)
														printf("%-20s ", header->next->booksInfo->isbn);
													if((result & 2)==2)
														printf("%-20s ", header->next->booksInfo->author);
													if((result & 4)==4)
														printf("%-20s ", header->next->booksInfo->title);
													if((result & 8)==8)
														printf("%-20s ", header->next->booksInfo->price);
													if((result & 16)==16)
														printf("%-20s ", header->next->booksInfo->subject);
													printf("\n");			
												}
											}
											else if(strcmp_ctrl(comp_1,attri_Name[4],0))
											{
												if(strcmp_ctrl(header->next->booksInfo->subject,comp_2,0))
												{
													// Success and print 
													if((result & 1)==1)
														printf("%-20s ", header->next->booksInfo->isbn);
													if((result & 2)==2)
														printf("%-20s ", header->next->booksInfo->author);
													if((result & 4)==4)
														printf("%-20s ", header->next->booksInfo->title);
													if((result & 8)==8)
														printf("%-20s ", header->next->booksInfo->price);
													if((result & 16)==16)
														printf("%-20s ", header->next->booksInfo->subject);
													printf("\n");			
												}
											}
											else
											{
												// When there don't have WHERE clause 
												if(strlen(comp_1) == 0)
												{
													if((result & 1)==1)
														printf("%-20s ", header->next->booksInfo->isbn);
													if((result & 2)==2)
														printf("%-20s ", header->next->booksInfo->author);
													if((result & 4)==4)
														printf("%-20s ", header->next->booksInfo->title);
													if((result & 8)==8)
														printf("%-20s ", header->next->booksInfo->price);
													if((result & 16)==16)
														printf("%-20s ", header->next->booksInfo->subject);
													printf("\n");		
												}
												else
												{
													printf("Not found this attribute-%s in table %s\n",comp_1,table_name_1);	
												}
											}
											header_w = header_w->next;
										} // WHERE clause
										header = header->next;
									} // for Buckets
								}
							
							break;
						}
						header2 = header2->next;
						count++;
					}
					printf("\n");
				}
				else
				{
					// comp2 != table2 (reject) , print out error
					printf("Error , don't have %s table!\n",comp2);
				}
			}
		}
		else if(strcmp_ctrl(comp1,table_name_2,0))
		{
			// comp1 = table2
			if(strcmp_ctrl(comp2,table_name_1,0))
			{
				// comp2 = table1 , do the tables attributes
				// TODO : when 2 table all get , need to check the join string
				printf("Match!\n");
			}
			else
			{	
				// Consider when comp2 empty
				if(strlen(comp2)<=1)
				{
					// Only need to consider comp1 , now get the SELECT strings 
					// Check for the Select clause : legal or not
					strLink *header2;
					header2 = forSelect;
					int count = 0;
					printf("Judge SELECT :");
					while(header2->next != NULL)
					{
						if(header2->next->name[0]=='*' && count == 0)
						{
							// If compare with this , check whether the other behind
							if(strlen(header2->next->name)>2)
							{
								printf("Error with string behind '*' , len is %d\n",(int)strlen(header2->next->name));
								return;
							}
							else
							{
								// Prepare the print out all the attributes of that table_1 !
								printf("List all attribute in table - %s :\n",table_name_2);
								printf("%-20s %-20s %-20s \n",attri_Name[5],attri_Name[6],attri_Name[7]);
								for(int j=0;j<10;j++)
								{
									attri_Value *header = &bucket_Header[5][j];
									while(header->next != NULL)
									{
										// Check for the WHERE clause : FETCH for GROUP and "=" for compare
										strLink *header_w = forWhere;
										while(header_w->next != NULL)
										{
											char comp_1[64];
											memset(comp_1,'\0',64*sizeof(char));
											char comp_2[64];
											memset(comp_2,'\0',64*sizeof(char));
											//sscanf(header_w->next->name,"%[^\n]", comp_1);
											sscanf(header_w->next->name,"%[^=]%*[=]%[^\n]",comp_1,comp_2);
											sscanf(comp_1,"%s",comp_1);
											
											if(strcmp_ctrl(comp_1,attri_Name[5],0))
											{
												if(strcmp_ctrl(header->next->sellsInfo->uid,comp_2,0))
												{
													// Success and print 
													printf("%-20s ", header->next->sellsInfo->uid);
													printf("%-20s ", header->next->sellsInfo->no);
													printf("%-20s \n", header->next->sellsInfo->isbn_no);			
												}
											}
											else if(strcmp_ctrl(comp_1,attri_Name[6],0))
											{
												if(strcmp_ctrl(header->next->booksInfo->author,comp_2,0))
												{
													// Success and print 
													printf("%-20s ", header->next->sellsInfo->uid);
													printf("%-20s ", header->next->sellsInfo->no);
													printf("%-20s \n", header->next->sellsInfo->isbn_no);				
												}
											}
											else if(strcmp_ctrl(comp_1,attri_Name[7],0))
											{
												if(strcmp_ctrl(header->next->booksInfo->title,comp_2,0))
												{
													// Success and print 
													printf("%-20s ", header->next->sellsInfo->uid);
													printf("%-20s ", header->next->sellsInfo->no);
													printf("%-20s \n", header->next->sellsInfo->isbn_no);			
												}
											}
											else
											{
												if(strlen(comp_1)==0)
												{
													printf("%-20s ", header->next->sellsInfo->uid);
													printf("%-20s ", header->next->sellsInfo->no);
													printf("%-20s \n", header->next->sellsInfo->isbn_no);
												}
												else
												{
													printf("Not found this attribute-%s in table %s\n",comp_2,table_name_2);	
												}
											}
											header_w = header_w->next;
										}
										header = header->next;
									}
								}
								
								break;
							}
						}
						else
						{
							// Do when only one table , and with selected 
							int result = judgeSelect(forSelect,1);
							printf("%d\n",result);
							if((result & 32)==32)
								printf("%-20s ",attri_Name[5]);
							if((result & 64)==64)
								printf("%-20s ",attri_Name[6]);
							if((result & 128)==128)
								printf("%-20s ",attri_Name[7]);
							printf("\n");
							
							for(int j=0;j<10;j++)
							{
								attri_Value *header = &bucket_Header[5][j];
								while(header->next != NULL)
								{
									// Check for the WHERE clause : FETCH for GROUP and "=" for compare
									strLink *header_w = forWhere;
									while(header_w->next != NULL)
									{
										char comp_1[64];
										memset(comp_1,'\0',64*sizeof(char));
										char comp_2[64];
										memset(comp_2,'\0',64*sizeof(char));
										sscanf(header_w->next->name,"%[^=] = %[^\n]",comp_1,comp_2);
										sscanf(comp_1,"%s",comp_1);
										
										//printf("comp1: %s , comp2: %s\n",comp_1,comp_2);
										
										if(strcmp_ctrl(comp_1,attri_Name[5],0))
										{
											if(strcmp_ctrl(header->next->sellsInfo->uid,comp_2,0))
											{
												// Success and print 
												if((result & 32)==32)
													printf("%-20s ",header->next->sellsInfo->uid);
												if((result & 64)==64)
													printf("%-20s ",header->next->sellsInfo->no);
												if((result & 128)==128)
													printf("%-20s ",header->next->sellsInfo->isbn_no);
												printf("\n");		
											}
										}
										else if(strcmp_ctrl(comp_1,attri_Name[6],0))
										{
											if(strcmp_ctrl(header->next->sellsInfo->no,comp_2,0))
											{
												// Success and print 
												if((result & 32)==32)
													printf("%-20s ",header->next->sellsInfo->uid);
												if((result & 64)==64)
													printf("%-20s ",header->next->sellsInfo->no);
												if((result & 128)==128)
													printf("%-20s ",header->next->sellsInfo->isbn_no);
												printf("\n");				
											}
										}
										else if(strcmp_ctrl(comp_1,attri_Name[7],0))
										{
											if(strcmp_ctrl(header->next->sellsInfo->isbn_no,comp_2,0))
											{
												// Success and print 
												if((result & 32)==32)
													printf("%-20s ",header->next->sellsInfo->uid);
												if((result & 64)==64)
													printf("%-20s ",header->next->sellsInfo->no);
												if((result & 128)==128)
													printf("%-20s ",header->next->sellsInfo->isbn_no);
												printf("\n");	
											}
										}
										else
										{
											// When there don't have WHERE clause 
											if(strlen(comp_1) == 0)
											{
												if((result & 32)==32)
													printf("%-20s ",header->next->sellsInfo->uid);
												if((result & 64)==64)
													printf("%-20s ",header->next->sellsInfo->no);
												if((result & 128)==128)
													printf("%-20s ",header->next->sellsInfo->isbn_no);
												printf("\n");			
											}
											else
											{
												printf("Not found this attribute-%s in table %s\n",comp_1,table_name_1);	
											}
										}
										header_w = header_w->next;
										memset(comp_1,'\0',64*sizeof(char));
										memset(comp_2,'\0',64*sizeof(char));
									} // WHERE clause
									header = header->next;
								} // for Buckets
							}
							
							break;
						}
						header2 = header2->next;
						count++;
					}
					printf("\n");
				}
				else
				{
					// comp2 != table1 (reject) , print out error
					printf("Error , don't have %s table!\n",comp2);
				}
			}
		}
		else{
			// comp1 compare to none of each tables (reject)
			printf("Error , don't have %s table!\n",comp1);
		}
		header = header->next;
	}
}

int judgeSelect(strLink *header,int mode)
{
	// Using 8 bits number to represent the attriName
	int result = 0;
	char string[64];
	while(header->next != NULL)
	{
		sscanf(header->next->name,"%s",string);
	// ==================================== For books ==================================== //
		if(mode == 0)
		{
			if(strcmp_ctrl(string,attri_Name[0],0))
			{
				if((result & 1) == 1)
				{
					// This attri have been record before , ignore it
				}
				else if((result & 1) == 0)
				{
					// Set result
					result += 1;
				}
			}
			else if(strcmp_ctrl(string,attri_Name[1],0))
			{
				if((result & 2) == 2)
				{
					// This attri have been record before , ignore it
				}
				else if((result & 2) == 0)
				{
					// Set result 
					result += 2;
				}
			}
			else if(strcmp_ctrl(string,attri_Name[2],0))
			{
				if((result & 4) == 4)
				{
					// This attri have been record before , ignore it
				}
				else if((result & 4) == 0)
				{
					// Set result 
					result += 4;
				}
			}
			else if(strcmp_ctrl(string,attri_Name[3],0))
			{
				if((result & 8) == 8)
				{
					// This attri have been record before , ignore it
				}
				else if((result & 8) == 0)
				{
					// Set result 
					result += 8;
				}
			}
			else if(strcmp_ctrl(string,attri_Name[4],0))
			{
				if((result & 16) == 16)
				{
					// This attri have been record before , ignore it
				}
				else if((result & 16) == 0)
				{
					// Set result 
					result += 16;
				}
			}
		}
		else if(mode == 1)
		{
	// ==================================== For sellRecord ==================================== //
			if(strcmp_ctrl(string,attri_Name[5],0))
			{
				if((result & 32) == 32)
				{
					// This attri have been record before , ignore it
				}
				else if((result & 32) == 0)
				{
					// Set result 
					result += 32;
				}
			}
			else if(strcmp_ctrl(string,attri_Name[6],0))
			{
				if((result & 64) == 64)
				{
					// This attri have been record before , ignore it
				}
				else if((result & 64) == 0)
				{
					// Set result 
					result += 64;
				}
			}
			else if(strcmp_ctrl(string,attri_Name[7],0))
			{
				if((result & 128) == 128)
				{
					// This attri have been record before , ignore it
				}
				else if((result & 128) == 0)
				{
					// Set result 
					result += 128;
				}
			}
		}
		header = header->next;
		memset(string,'\0',64*sizeof(char));
	}
	
	return result;
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
	        bucket_Header[i][j].booksInfo = NULL;
	        bucket_Header[i][j].sellsInfo = NULL;
	    }
	}
	// Read the resource data
	FILE *fp;
	fp = fopen(FILEBOOK,"r");
	// Store for table name
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
                printf(" and %s ", header->next->booksInfo->isbn);
                printf(" and %s ", header->next->sellsInfo->uid);
                header = header->next;
            }
            printf("\n");
        }
    }   
    // For struct debugging
	books *header1;
	header1 = books_Header;
	printf("Books Header:\n");
	// First header is empty
	while(header1->next != NULL)
	{
		printf(",%s,%s,%s,%s,%s \n",header1->next->isbn,header1->next->author,header1->next->title,header1->next->price,header1->next->subject);	
		header1 = header1->next;
	}

	sells *header2 = sells_Header;
	printf("Sells Header:\n");
	// First header is empty
	while(header2->next != NULL)
	{
		printf(",%s,%s,%s \n",header2->next->uid,header2->next->no,header2->next->isbn_no);	
		header2 = header2->next;
	}
}

void printSQL()
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
