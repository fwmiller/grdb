#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "graph.h"

void cli_graph_new(char *cmdline, int *pos);
void cli_graph_component(char *cmdline, int *pos);
void cli_graph_edge(char *cmdline, int *pos);
void cli_graph_schema(char *cmdline, int *pos);
void cli_graph_tuple(char *cmdline, int *pos);

static void
cli_components_print(graph_t g, int gno)
{
	component_t c;
	int ccnt = 0;

	for (c = g->c; c != NULL; c = c->next, ccnt++) {
		if (g == current_graph && c == current_component)
			printf(">");

		printf("%d.%d:", gno, ccnt);
		component_print(c, 0); /* no tuples */
		printf("\n");
	}
}

static void
cli_graph_print()
{
	graph_t g;
	int gcnt = 0;

	for (g = graphs; g != NULL; g = g->next, gcnt++)
		cli_components_print(g, gcnt);
}

void
cli_graph(char *cmdline, int *pos)
{
	char s[BUFSIZE];

	assert (cmdline != NULL);
	assert (pos != NULL);

	memset(s, 0, BUFSIZE);
	nextarg(cmdline, pos, " ", s);

	if (strcmp(s, "new") == 0 || strcmp(s, "n") == 0)
		cli_graph_new(cmdline, pos);

	else if (strcmp(s, "component") == 0 || strcmp(s, "c") == 0)
		cli_graph_component(cmdline, pos);

	else if (strcmp(s, "edge") == 0 || strcmp(s, "e") == 0)
		cli_graph_edge(cmdline, pos);

	else if (strcmp(s, "schema") == 0 || strcmp(s, "s") == 0)
		cli_graph_schema(cmdline, pos);

	else if (strcmp(s, "tuple") == 0 || strcmp(s, "t") == 0)
		cli_graph_tuple(cmdline, pos);

	else if (isdigit(s[0])) {
		graph_t g;
		component_t c;
		int cno, gno, ccnt, gcnt, spos;
		char s1[BUFSIZE];
#if _DEBUG
		printf("s=[%s] ", s);
#endif
		spos = 0;
		memset(s1, 0, BUFSIZE);
		nextarg(s, &spos, ".", s1);
#if _DEBUG
		printf("s1=[%s] ", s1);
#endif
		gno = atoi(s1);
#if _DEBUG
		printf("gno=%d ", gno);
#endif
		spos++;
		memset(s1, 0, BUFSIZE);
		nextarg(cmdline, pos, " ", s1);
#if _DEBUG
		printf("s1=[%s] ", s1);
#endif

		cno = atoi(s1);
#if _DEBUG
printf("gno=%d\n", cno);
#endif
		for (g = graphs, gcnt = 0; g != NULL; g = g->next, gcnt++)
			for (c = g->c, ccnt = 0; c != NULL; c = c->next, ccnt++)
				if (gcnt == gno && ccnt == cno) {
					current_graph = g;
					current_component = c;
					return;
				}

		
	} else if (strlen(s) == 0)
		cli_graph_print();
}
