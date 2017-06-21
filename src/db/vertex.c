#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"

/* Clear the vertex data structure */
void
vertex_init(vertex_t v)
{
	assert(v != NULL);
	memset(v, 0, sizeof(struct vertex));
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

	/*
	 * Check whether a tuple exists on secondary storage for this id.
	 * If so, allocate a tuple structure and initialize it if necessary
	 * and read the tuple from secondary storage.
	 */
}

void
vertex_print(vertex_t v)
{
	assert(v != NULL);
	printf("%llu", v->id);
}

/*
 * Read the tuple associated with the vertex id from secondary storage.
 * Assume the id is set and the tuple data structure is allocated but
 * not initialized.
 */
ssize_t
vertex_read(vertex_t v)
{
	assert(v != NULL);
	assert(v->tuple != NULL);

	return 0;
}

/*
 * Write the tuple associated with the vertex id to secondary storage.
 * Assume the id and tuple are set to be written.
 */
ssize_t
vertex_write(vertex_t v)
{
	assert(v != NULL);
	assert(v->tuple != NULL);

	return 0;
}
