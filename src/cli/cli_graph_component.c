#if _DEBUG
#include <errno.h>
#endif
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include "cli.h"
#include "graph.h"

static void
cli_graph_component_new()
{
	struct vertex v;
	char s[BUFSIZE];
	int fd, n;

	n = graph_next_cno(grdbdir, gno);
	if (n < 0) {
#if _DEBUG
		printf("cli_graph_component_new: bad next cno\n");
#endif
		return;
	}
	/* Create first vertex in component */
	vertex_init(&v);
	vertex_set_id(&v, 1);

	/* Persistence... */
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d", grdbdir, gno, n);
	mkdir(s, 0755);

	/* Create component vertex file */
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/v", grdbdir, gno, n);
#if _DEBUG
	printf("cli_graph_component_new: open vertex file %s\n", s);
#endif
	fd = open(s, O_RDWR | O_CREAT, 0644);
	if (fd < 0)
		return;

	/* Write first vertex tuple */
	vertex_write(&v, fd);
	close(fd);
}

static void
cli_graph_component_sssp(char *cmdline, int *pos)
{
	struct component c;
	vertexid_t v1, v2;
	char s[BUFSIZE];
	int fd, n, total_weight, result;
	vertexid_t *path = NULL;

	memset(s, 0, BUFSIZE);
	nextarg(cmdline, pos, " ", s);
	v1 = (vertexid_t) atoi(s);

	memset(s, 0, BUFSIZE);
	nextarg(cmdline, pos, " ", s);
	v2 = (vertexid_t) atoi(s);

	/* XXX Need to do some error checking on v1 and v2 here */

#if _DEBUG
	printf("cli_graph_component_sssp: ");
	printf("execute dijkstra on vertex ids %llu and %llu\n", v1, v2);
#endif

	/* Initialize component */
	component_init(&c);

	/* Load enums */
	fd = enum_file_open(grdbdir, gno, cno);
	if (fd >= 0) {
		enum_list_init(&(c.el));
		enum_list_read(&(c.el), fd);
		close(fd);
	}
	/* Load the edge schema */
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/se", grdbdir, gno, cno);
#if _DEBUG
	printf("cli_graph_component_sssp: ");
	printf("read edge schema file %s\n", s);
#endif
	fd = open(s, O_RDONLY);
	if (fd < 0) {
		printf("Component must have an edge schema\n");
		return;
	}
	c.se = schema_read(fd, c.el);
	close(fd);

	/* Setup and run Dijkstra */
	n = (-1);
	total_weight = (-1);
	result = component_sssp(&c, v1, v2, &n, &total_weight, &path);
	if (result < 0) {
		/* Failure... */
	}
}

static void
cli_graph_component_project(char *cmdline, int *pos)
{
	char s[BUFSIZE];

	/* Get list of attributes */

	memset(s, 0, BUFSIZE);
	nextarg(cmdline, pos, " ", s);
}

static void
cli_graph_component_select(char *cmdline, int *pos)
{
#if _DEBUG
	printf("cli_graph_component_select: select failed\n");
#endif
}

static void
cli_graph_component_union(char *cmdline, int *pos)
{
	int result;
	char s[BUFSIZE];
	int cidx1 = (-1), cidx2 = (-1);

	/* Get first component argument */
	memset(s, 0, BUFSIZE);
	nextarg(cmdline, pos, " ", s);
#if _DEBUG
	printf("cli_graph_component_union: first component %s\n", s);
#endif
	cidx1 = atoi(s);

	/* Get second component argument */
	memset(s, 0, BUFSIZE);
	nextarg(cmdline, pos, " ", s);
#if _DEBUG
	printf("cli_graph_component_union: second component %s\n", s);
#endif
	cidx2 = atoi(s);

	result = component_union(cidx1, cidx2, grdbdir, gno);
	if (result < 0) {
#if _DEBUG
		printf("cli_graph_component_union: union failed\n");
#endif
	}
}

void
cli_graph_component(char *cmdline, int *pos)
{
	char s[BUFSIZE];

	memset(s, 0, BUFSIZE);
	nextarg(cmdline, pos, " ", s);

	if (strcmp(s, "new") == 0 || strcmp(s, "n") == 0)
		cli_graph_component_new();

	else if (strcmp(s, "sssp") == 0)
		cli_graph_component_sssp(cmdline, pos);

	else if (strcmp(s, "project") == 0 || strcmp(s, "p") == 0)
		cli_graph_component_project(cmdline, pos);

	else if (strcmp(s, "select") == 0 || strcmp(s, "s") == 0)
		cli_graph_component_select(cmdline, pos);

	else if (strcmp(s, "union") == 0 || strcmp(s, "u") == 0)
		cli_graph_component_union(cmdline, pos);

	else if (strlen(s) == 0) {
		FILE *out;
		char s[BUFSIZE];

		memset(s, 0, BUFSIZE);
		sprintf(s, "/tmp/grdbGraphs");
		out = fopen(s, "w");
		if (out == NULL) {
			printf("cli_graph_component: fopen %s failed\n", s);
			return;
		}
		memset(s, 0, BUFSIZE);
		sprintf(s, "%d", gno);
		cli_components_print(out, s, 0); /* no tuples */

		fclose(out);
	}
}
