#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#if _DEBUG
#include <stdio.h>
#endif
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "cli.h"

static void
cli_graphs_insert(graph_t g)
{
	graph_t f;

	if (graphs == NULL) {
		graphs = g;
		current_graph = g;
		current_component = g->c;
		return;
	}
	for (f = graphs; f->next != NULL; f = f->next);
	f->next = g;
}

void
cli_graph_new(char *cmdline, int *pos)
{
	graph_t g;
	component_t c;
	vertex_t v;
	int gidx, cidx;
	char s[BUFSIZE];

	/* Create first vertex in component */
	v = (vertex_t) malloc(sizeof(struct vertex));
	assert (v != NULL);
	vertex_init(v);
	vertex_set_id(v, 1);

	/* Create new component */
	c = (component_t) malloc(sizeof(struct component));
	assert (c != NULL);
	component_init(c);
	component_insert_vertex(c, v);

	/* Create new graph */
	g = (graph_t) malloc(sizeof(struct graph));
	assert (g != NULL);
	graph_init(g);
	graph_insert_component(g, c);

	cli_graphs_insert(g);

	/* Persistence... */
	gidx = graphs_get_current_index();
	cidx = components_get_index(current_graph);

	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d", grdbdir, gidx);
	mkdir(s, 0755);

	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d", grdbdir, gidx, cidx);
	mkdir(s, 0755);

	/* Create component vertex file */
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/v", grdbdir, gidx, cidx);
#if _DEBUG
	printf("cli_graph_new: open vertex file %s\n", s);
#endif
	c->vfd = open(s, O_RDWR | O_CREAT, 0644);
	if (c->vfd < 0) {
#if _DEBUG
		printf("cli_graph_new: open vertex file failed (%s)\n",
			strerror(errno));
#endif
		return;
	}
	/* Write first vertex tuple */
	vertex_write(v, c->vfd);
}
