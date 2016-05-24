#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// FIXME : C90 doesn't support boolean
#include <stdbool.h>

typedef struct stringLink{
	char name[64];
	struct stringLink *next;
} strLink;

strLink *forSelect;
strLink *forFrom;
strLink *forWhere;
// ** For string operation ** //

// ** For fetching command ** //
int fetch_cmd();
char* read_cmd();
int parse_cmd(char *cmd);
void AddStrLink(strLink *header , char *name);
void dealClause(strLink *header , char *name);
void releaseAll(strLink *header);
