#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "config.h"
#include "graph.h"
#include "tuple.h"

void
component_init(component_t c)
{
	assert (c != NULL);
	memset(c, 0, sizeof(struct component));

	/* Initial values for file descriptors */
	c->vfd = (-1);
	c->efd = (-1);
}

void
component_file_init(int gidx, int cidx)
{
	char s[BUFSIZE];

	/* Assume graph file initialization is done */
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d", GRDBDIR, gidx, cidx);
	mkdir(s, 0755);
}

vertex_t
component_find_vertex_by_id(component_t c, vertex_t v)
{
	int len;

	assert (c != NULL);
	assert (v != NULL);

	/* Assume v was allocated and v->id was set by the caller */
	len = vertex_read(v, c->vfd);
	if (len > 0)
		return v;

	return NULL;
}

void
component_insert_vertex(component_t c, vertex_t v)
{
	assert (c != NULL);
	assert (v != NULL);

	vertex_write(v, c->vfd);
}

edge_t
component_find_edge_by_ids(component_t c, edge_t e)
{
	int len;

	assert (c != NULL);
	assert (e != NULL);

	/*
	 * Assume e was allocated and e->id1 and e->id2 were set by
	 * the caller
	 */
	len = edge_read(e, c->efd);
	if (len > 0)
		return e;

	return NULL;
}

void
component_insert_edge(component_t c, edge_t e)
{
	assert (c != NULL);
	assert (e != NULL);

	edge_write(e, c->efd);
}

void
component_print(component_t c, int with_tuples)
{
	off_t off;
	ssize_t len, size;
	vertexid_t id, id1, id2;
	tuple_t tuple;
	char *buf;

	assert (c != NULL);

	printf("({");

	/* Vertices */
	if (c->sv == NULL)
		size = 0;
	else
		size = schema_size(c->sv);

	buf = malloc((sizeof(vertexid_t) << 1) + size);

	for (off = 0;; off += sizeof(vertexid_t) + size) {
		lseek(c->vfd, off, SEEK_SET);
		len = read(c->vfd, buf, sizeof(vertexid_t) + size);
		if (len <= 0)
			break;

		id = *((vertexid_t *) buf);
		printf("%llu", id);
		if (off > 0)
			printf(",");

		if (c->sv != NULL) {
			tuple = (tuple_t) (buf + sizeof(vertexid_t));
			tuple_print(tuple, c->el);
		}
	}
	printf("},{");

	/* Edges */
	if (c->se == NULL)
		size = 0;
	else
		size = schema_size(c->se);

	for (off = 0;; off += (sizeof(vertexid_t) << 1) + size) {
		lseek(c->efd, off, SEEK_SET);
		len = read(c->efd, buf, (sizeof(vertexid_t) << 1) + size);
		if (len <= 0)
			break;

		id1 = *((vertexid_t *) buf);
		id2 = *((vertexid_t *) (buf + sizeof(vertexid_t)));
		printf("(%llu,%llu)", id1, id2);
		if (off > 0)
			printf(",");

		if (c->se != NULL) {
			tuple = (tuple_t) (buf + (sizeof(vertexid_t) << 1));
			tuple_print(tuple, c->el);
		}
	}
	printf("})");
}
