#include "cli.h"

void
cli_graph_import(char *cmdline, int *pos)
{
	char s[BUFSIZE];

	/* Get filename to import  */
	memset(s, 0, BUFSIZE);
	nextarg(cmdline, pos, " ", s);
#if _DEBUG
	printf("cli_graph_import: import graph file %s\n", s);
#endif
}
