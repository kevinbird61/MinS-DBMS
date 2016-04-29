#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// FIXME : C90 doesn't support boolean
#include <stdbool.h>
// ** For string operation ** //

// ** For fetching command ** //
void fetch_cmd();
char* read_cmd();
int parse_cmd(char *cmd);
