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
	struct component c;
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
	printf("add edge (%d,%d) to component %d.%d\n", i, j, gno, cno);
#endif
	vertex_init(&v);
	vertex_set_id(&v, i);

	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/v", grdbdir, gno, cno);
#if _DEBUG
	printf("cli_graph_edge: ");
	printf("open component file %s\n", s);
#endif
	component_init(&c);
	c.vfd = open(s, O_RDWR);
	v1 = component_find_vertex_by_id(&c, &v);
	close(c.vfd);

#if _DEBUG
	if (v1 == NULL)
		printf("cli_graph_edge: vertex %d not found\n", i);
#endif
	vertex_init(&w);
	vertex_set_id(&w, j);

	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/v", grdbdir, gno, cno);
#if _DEBUG
	printf("cli_graph_edge: ");
	printf("open component file %s\n", s);
#endif
	component_init(&c);
	c.vfd = open(s, O_RDWR);
	w1 = component_find_vertex_by_id(&c, &w);
	close(c.vfd);

#if _DEBUG
	if (w1 == NULL)
		printf("cli_graph_edge: vertex %d not found\n", j);
#endif
	if (v1 == NULL && w1 == NULL) {
		printf("At least one vertex must exist in component\n");
		return;
	}

	/* Add the new vertex */
	if (v1 == NULL || w1 == NULL) {
		memset(s, 0, BUFSIZE);
		sprintf(s, "%s/%d/%d/v", grdbdir, gno, cno);
#if _DEBUG
		printf("cli_graph_edge: open vertex file %s\n", s);
#endif
		fd = open(s, O_RDWR);
		if (fd < 0) {
#if _DEBUG
			printf("cli_graph_edge: ");
			printf("open vertex file failed (%s)\n",
				strerror(errno));
#endif
			return;
		}
		vertex_init(&v);
		vertex_set_id(&v, (v1 == NULL ? i : j));
		vertex_write(&v, fd);
		close(fd);
	}

	/* Add the new edge */
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
