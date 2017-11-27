#include <assert.h>
#if _DEBUG
#include <errno.h>
#endif
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include "config.h"
#include "graph.h"

/* Clear the vertex data structure */
void
vertex_init(vertex_t v)
{
	assert(v != NULL);
	memset(v, 0, sizeof(struct vertex));
}

/* Initialize the vertex file */
int
vertex_file_init(int gidx, int cidx)
{
	char s[BUFSIZE];
	int fd;

	/* Create component vertex file */
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/v", GRDBDIR, gidx, cidx);
#if _DEBUG
	printf("vertex_file_init: open vertex file %s\n", s);
#endif
	fd = open(s, O_RDWR | O_CREAT, 0644);
#if _DEBUG
	if (fd < 0) {
		printf("vertex_file_init: open vertex file failed (%s)\n",
			strerror(errno));
	}
#endif
	return fd;
}

/*
 * Set the vertex id for a vertex data structure.  This action associates
 * this vertex data structure with tuple for the vertex id stored on
 * secondary storage.
 */
void
vertex_set_id(vertex_t v, vertexid_t id)
{
	assert(v != NULL);
	v->id = id;
}

void
vertex_print(vertex_t v)
{
	assert(v != NULL);
	printf("%llu", v->id);
}
