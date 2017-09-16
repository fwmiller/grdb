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

	component_init(&c);

	/* Load enums */
	fd = enum_file_open(grdbdir, gno, cno);
	if (fd >= 0) {
		enum_list_init(&(c.el));
		c.el = enum_list_read(&(c.el), fd);
		close(fd);
	}
	/* Load vertex schema */
	schema_init(&(c.sv));
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/sv", grdbdir, gno, cno);
	fd = open(s, O_RDWR);
	if (fd >= 0) {
		c.sv = schema_read(fd, c.el);
		close(fd);
	}
	/* Load edge schema */
	schema_init(&(c.se));
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/se", grdbdir, gno, cno);
	fd = open(s, O_RDWR);
	if (fd >= 0) {
		c.se = schema_read(fd, c.el);
#if _DEBUG
		if (c.se == NULL)
			printf("cli_graph_edge: edge schema is NULL\n");
#endif
		close(fd);
	}
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/v", grdbdir, gno, cno);
#if _DEBUG
	printf("cli_graph_edge: open vertex file %s\n", s);
#endif
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
	printf("open vertex file %s\n", s);
#endif
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
		if (c.sv != NULL)
			tuple_init(&(v.tuple), c.sv);
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
	if (c.se == NULL) {
#if _DEBUG
		printf("cli_graph_edge: edge schema is NULL\n");
#endif
	} else {
#if _DEBUG
		printf("cli_graph_edge: edge schema is NOT NULL\n");
#endif
		tuple_init(&(e.tuple), c.se);
		if (e.tuple == NULL) {
#if _DEBUG
			printf("cli_graph_edge: tuple is NULL\n");
#endif
		}
	}
	edge_write(&e, fd);
	close(fd);
}
