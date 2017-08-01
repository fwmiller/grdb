#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "config.h"
#include "graph.h"

void
graph_init(graph_t g)
{
	assert (g != NULL);
	memset(g, 0, sizeof(struct graph));
}

void
graph_file_init(int gidx)
{
	char s[BUFSIZE];

	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d", GRDBDIR, gidx);
	mkdir(s, 0755);
}

void
graph_insert_component(graph_t g, component_t c)
{
	component_t d;

	if (g->c == NULL) {
		g->c = c;
		return;
	}
	for (d = g->c; d->next != NULL; d = d->next);
	d->next = c;
}
