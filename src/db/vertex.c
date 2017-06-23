#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "graph.h"
#include "tuple.h"

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
 * The vertex file is arranged as a packed list of vertex records.  Each
 * record contains a 64-bit vertex id followed by the vertex tuple.
 */

ssize_t
vertex_read(vertex_t v, int fd)
{
	off_t off;
	ssize_t len, size;
	vertexid_t id;

	assert(v != NULL);
	assert(v->tuple != NULL);

	size = schema_size(v->tuple->s);

	/* Search for vertex id in current component */
	for (off = 0;; off += sizeof(vertexid_t) + size) {
		/*
		 * Use the tuple buffer for temporary storage to hold the
		 * read vertex id bytes.
		 */
		lseek(fd, off, SEEK_SET);
		len = read(fd, v->tuple->buf, sizeof(vertexid_t));
		if (len != sizeof(vertexid_t))
			return (-1);

		id = *((vertexid_t *) v->tuple->buf);
		if (id == v->id) {
			memset(v->tuple->buf, 0, size);
			return read(fd, v->tuple->buf, size);
		}
	}
	return (-1);
}

/*
 * Write the tuple record to secondary storage.  The record contains a
 * 64-bit vertex id followed by the vertex tuple.  Assume the id and tuple
 * are set to be written.
 */
ssize_t
vertex_write(vertex_t v, int fd)
{
	off_t off;
	ssize_t len, size;
	vertexid_t id;

	assert(v != NULL);
	assert(v->tuple != NULL);

	size = schema_size(v->tuple->s);

	/* Search for vertex id in current component */
	for (off = 0;; off += sizeof(vertexid_t) + size) {
		lseek(fd, off, SEEK_SET);
		len = read(fd, v->tuple->buf, sizeof(vertexid_t));
		if (len == 0)
			/* EOF reached */
			break;

		if (len != sizeof(vertexid_t))
			return (-1);

		id = *((vertexid_t *) v->tuple->buf);
		if (id == v->id) {
			/*
			 * The vertex id is already on secondary storage
			 * so just "drop the head" and update the tuple
			 */
			memset(v->tuple->buf, 0, size);
			return write(fd, v->tuple->buf, size);
		}
	}
	/*
	 * The vertex id was not found in the search so "drop the head"
	 * and write the vertex id and tuple buffer.
	 */
	len = write(fd, &(v->id), sizeof(vertexid_t));
	if (len != sizeof(vertexid_t))
		return (-1);

	return write(fd, v->tuple->buf, size);
}
