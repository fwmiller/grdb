#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "graph.h"

void
cli_graph_component(char *cmdline, int *pos)
{
	char s[BUFSIZE];

	memset(s, 0, BUFSIZE);
	nextarg(cmdline, pos, " ", s);

	if (strcmp(s, "new") == 0 || strcmp(s, "n") == 0) {
		component_t c;
		vertex_t v;

		/* Create first vertex in component */
		v = (vertex_t) malloc(sizeof(struct vertex));
		assert (v != NULL);
		vertex_init(v);
		v->id = 1;

		/* Create new component */
		c = (component_t) malloc(sizeof(struct component));
		assert (c != NULL);
		component_init(c);
		component_insert_vertex(c, v);

		graph_insert_component(current_graph, c);

	} else if (strlen(s) == 0)
		cli_components_print(current_graph,
				     graphs_get_current_index());
}
