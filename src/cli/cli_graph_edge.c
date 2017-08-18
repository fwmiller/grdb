#include <assert.h>
#if _DEBUG
#include <errno.h>
#endif
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "cli.h"

void
cli_graph_edge(char *cmdline, int *pos)
{
	struct vertex v, w;
	vertex_t v1, w1;
	struct edge e;
	char s[BUFSIZE];
	int fd, i, j;

	/* Get the vertex id arguments */
	memset(s, 0, BUFSIZE);
	nextarg(cmdline, pos, " ", s);
	if (strlen(s) == 0) {
		printf("Missing vertex id\n");
		return;
	}
	i = atoi(s);

	memset(s, 0, BUFSIZE);
	nextarg(cmdline, pos, " ", s);
	if (strlen(s) == 0) {
		printf("Missing vertex id\n");
		return;
	}
	j = atoi(s);
#if _DEBUG
	printf("cli_graph_edge: ");
	printf("add edge (%d,%d) to current component\n", i, j);
#endif
	vertex_init(&v);
	vertex_set_id(&v, i);
	v1 = component_find_vertex_by_id(current_component, &v);
#if _DEBUG
	if (v1 == NULL)
		printf("cli_graph_edge: vertex %d not found\n", i);
#endif
	vertex_init(&w);
	vertex_set_id(&w, j);
	w1 = component_find_vertex_by_id(current_component, &w);
#if _DEBUG
	if (w1 == NULL)
		printf("cli_graph_edge: vertex %d not found\n", j);
#endif
	if (v1 == NULL && w1 == NULL) {
		printf("At least one vertex must exist in component\n");
		return;
	}
	/* Persistence... */
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/e", grdbdir, gno, cno);
#if _DEBUG
	printf("cli_graph_edge: open edge file %s\n", s);
#endif
	fd = open(s, O_RDWR | O_CREAT, 0644);
	if (fd < 0) {
#if _DEBUG
		printf("cli_graph_edge: ");
		printf("open edge file failed (%s)\n", strerror(errno));
#endif
		return;
	}
	edge_init(&e);
	edge_set_vertices(&e, i, j);
	edge_write(&e, fd);
	close(fd);
}
