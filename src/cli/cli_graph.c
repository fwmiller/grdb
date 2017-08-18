#include <assert.h>
#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include "cli.h"
#include "graph.h"

void cli_graph_new(char *cmdline, int *pos);
void cli_graph_component(char *cmdline, int *pos);
void cli_graph_edge(char *cmdline, int *pos);
void cli_graph_schema(char *cmdline, int *pos);
void cli_graph_tuple(char *cmdline, int *pos);

void
cli_components_print(char *gname)
{
	char s[BUFSIZE];
	DIR *fd;
	struct dirent *de;

	/*
	 * Loop over directories in the graph to display each
	 * component
	 */
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%s", grdbdir, gname);
#if _DEBUG
	printf("cli_components_print: directory %s\n", s);
#endif
	if ((fd = opendir(s)) == NULL)
		return;

	for (;;) {
		de = readdir(fd);
		if (de == NULL)
			break;

		if (strcmp(de->d_name, ".") != 0 &&
		    strcmp(de->d_name, "..") != 0) {
#if _DEBUG
			printf("cli_components_print: ");
			printf("component %s\n", de->d_name);
#endif
		}
	}
	closedir(fd);
}

void
cli_graphs_print()
{
	DIR *fd;
	struct dirent *de;

	/*
	 * Loop over directories in the grdb directory to display each
	 * graph
	 */
#if _DEBUG
	printf("cli_graphs_print: directory %s\n", grdbdir);
#endif
	if ((fd = opendir(grdbdir)) == NULL)
		return;

	for (;;) {
		de = readdir(fd);
		if (de == NULL)
			break;

		if (strcmp(de->d_name, ".") != 0 &&
		    strcmp(de->d_name, "..") != 0) {
#if _DEBUG
			printf("cli_graphs_print: graph %s\n", de->d_name);
#endif
			cli_components_print(de->d_name);
		}
	}
	closedir(fd);
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
		int cidx, gidx, spos;
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
		gidx = atoi(s1);
#if _DEBUG
		printf("gidx=%d ", gidx);
#endif
		spos++;
		memset(s1, 0, BUFSIZE);
		nextarg(s, &spos, " ", s1);
#if _DEBUG
		printf("s1=[%s] ", s1);
#endif
		cidx = atoi(s1);
#if _DEBUG
		printf("cidx=%d\n", cidx);
#endif
		gno = gidx;
		cno = cidx;
		
	}
	if (strlen(s) == 0)
		cli_graphs_print();
}
