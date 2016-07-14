#include <stdio.h>
#include <string.h>
#include "cli.h"

void
cli_graph_tuple(char *cmdline, int *pos)
{
	char s[BUFSIZE];

	memset(s, 0, BUFSIZE);
	nextarg(cmdline, pos, " ", s);

	graph_print(current, 1); /* with tuples */
	printf("\n");
}
