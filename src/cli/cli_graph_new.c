#include <assert.h>
#include <stdlib.h>
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
	ssize_t len;
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

	/* XXX Persistence... */

	/* Check for graph directory */

	//  XXX   gidx = graphs_get_current_index();

	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d", grdbdir, gidx);
#if _DEBUG
	printf("cli_graph_new: graph directory %s\n", s);
#endif

	/* Check for existing component vertex file */

	/* Create component vertex file */

	/* Write first vertex tuple */
	vertex_write(v, c->vfd);
}
