#include <assert.h>
#include <fcntl.h>
#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
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
	DIR *dirfd;
	struct dirent *de;
	struct component c;

	/*
	 * Loop over directories in the graph to display each
	 * component
	 */
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%s", grdbdir, gname);
#if _DEBUG
	printf("cli_components_print: directory %s\n", s);
#endif
	if ((dirfd = opendir(s)) == NULL)
		return;

	for (;;) {
		de = readdir(dirfd);
		if (de == NULL)
			break;

		if (strcmp(de->d_name, ".") != 0 &&
		    strcmp(de->d_name, "..") != 0) {
#if _DEBUG
			printf("cli_components_print: ");
			printf("component %s\n", de->d_name);
#endif
			printf("%s.%s:", gname, de->d_name);

			component_init(&c);

			/* XXX Load vertex schema */

			/* XXX Load edge schema */

			/* XXX Load enums schema if any */

			/* Open vertex file */
			memset(s, 0, BUFSIZE);
			sprintf(s, "%s/%s/%s/v", grdbdir, gname, de->d_name);
			c.vfd = open(s, O_RDWR);

			/* Open edge file */
			memset(s, 0, BUFSIZE);
			sprintf(s, "%s/%s/%s/e", grdbdir, gname, de->d_name);
			c.efd = open(s, O_RDWR);

			component_print(&c, 0); /* no tuples */
			printf("\n");

			/* Close files */
			close(c.efd);
			close(c.vfd);
		}
	}
	closedir(dirfd);
}

void
cli_graphs_print()
{
	DIR *dirfd;
	struct dirent *de;

	/*
	 * Loop over directories in the grdb directory to display each
	 * graph
	 */
#if _DEBUG
	printf("cli_graphs_print: directory %s\n", grdbdir);
#endif
	if ((dirfd = opendir(grdbdir)) == NULL)
		return;

	for (;;) {
		de = readdir(dirfd);
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
	closedir(dirfd);
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
