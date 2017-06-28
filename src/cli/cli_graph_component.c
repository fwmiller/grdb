#include <assert.h>
#if _DEBUG
#include <errno.h>
#endif
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
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
		int gidx, cidx;

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

		/* Persistence... */
		gidx = graphs_get_index(current_graph);
		cidx = components_get_index(current_graph, c);

		memset(s, 0, BUFSIZE);
		sprintf(s, "%s/%d/%d", grdbdir, gidx, cidx);
		mkdir(s, 0755);

		/* Create component vertex file */
		memset(s, 0, BUFSIZE);
		sprintf(s, "%s/%d/%d/v", grdbdir, gidx, cidx);
#if _DEBUG
		printf("cli_graph_component: open vertex file %s\n", s);
#endif
		c->vfd = open(s, O_RDWR | O_CREAT, 0644);
		if (c->vfd < 0) {
#if _DEBUG
			printf("cli_graph_component: ");
			printf("open vertex file failed (%s)\n",
				strerror(errno));
#endif
			return;
		}
		/* Write first vertex tuple */
		vertex_write(v, c->vfd);

	} else if (strlen(s) == 0)
		cli_components_print(current_graph,
				     graphs_get_index(current_graph));
}
