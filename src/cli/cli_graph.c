#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "cli.h"
#include "graph.h"

void cli_graph_import(char *cmdline, int *pos);
void cli_graph_new(char *cmdline, int *pos);
void cli_graph_component(char *cmdline, int *pos);
void cli_graph_edge(char *cmdline, int *pos);
void cli_graph_schema(char *cmdline, int *pos);
void cli_graph_tuple(char *cmdline, int *pos);

void
cli_graph(char *cmdline, int *pos)
{
	char s[BUFSIZE];
	struct stat sbuf;

	assert (cmdline != NULL);
	assert (pos != NULL);

	memset(s, 0, BUFSIZE);
	nextarg(cmdline, pos, " ", s);

	if (strlen(s) == 0) {
		cli_graphs_print();
		return;

	} else if (strcmp(s, "import") == 0 || strcmp(s, "i") == 0) {
		cli_graph_import(cmdline, pos);
		return;

	} else if (strcmp(s, "new") == 0 || strcmp(s, "n") == 0) {
		cli_graph_new(cmdline, pos);
		return;

	} else if (strcmp(s, "component") == 0 || strcmp(s, "c") == 0) {
		cli_graph_component(cmdline, pos);
		return;

	} else if (strcmp(s, "edge") == 0 || strcmp(s, "e") == 0) {
		cli_graph_edge(cmdline, pos);
		return;

	} else if (strcmp(s, "enum") == 0) {
		cli_graph_enum(cmdline, pos);
		return;

	} else if (strcmp(s, "schema") == 0 || strcmp(s, "s") == 0) {
		cli_graph_schema(cmdline, pos);
		return;

	} else if (strcmp(s, "tuple") == 0 || strcmp(s, "t") == 0) {
		cli_graph_tuple(cmdline, pos);
		return;

	} else if (isdigit(s[0])) {
		int cidx, gidx, spos;
		char s1[BUFSIZE], s2[BUFSIZE];
		int result;
#if _DEBUG
		printf("s=[%s] ", s);
#endif
		spos = 0;
		memset(s1, 0, BUFSIZE);
		nextarg(s, &spos, ".", s1);
#if _DEBUG
		printf("s1=[%s] ", s1);
#endif
		gidx = atoi(s1);
#if _DEBUG
		printf("gidx=%d ", gidx);
#endif
		spos++;
		memset(s2, 0, BUFSIZE);
		nextarg(s, &spos, " ", s2);
#if _DEBUG
		printf("s2=[%s] ", s2);
#endif
		cidx = atoi(s2);
#if _DEBUG
		printf("cidx=%d\n", cidx);
#endif
		/*
		 * Check whether specified graph and component
		 * number exist
		 */
		memset(s, 0, BUFSIZE);
		sprintf(s, "%s/%s/%s", grdbdir, s1, s2);
		memset(&sbuf, 0, sizeof(struct stat));
		result = stat(s, &sbuf);
		if (result < 0) {
			printf("component %d.%d does not exist\n",
				gidx, cidx);
			return;
		}
		gno = gidx;
		cno = cidx;
		return;
	}
}
