#include <assert.h>
#if _DEBUG
#include <stdio.h>
#endif
#include <stdlib.h>
#include <unistd.h>
#include "graph.h"

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
	char buf[sizeof(vertexid_t)];

	assert(v != NULL);
#if _DEBUG
	printf("vertex_write: write vertex %llu\n", v->id);
#endif
	if (v->tuple == NULL)
		size = 0;
	else
		size = schema_size(v->tuple->s);
#if _DEBUG
	printf("vertex_write: schema size = %lu bytes\n", size);
#endif

	/* Search for vertex id in current component */
	for (off = 0;; off += sizeof(vertexid_t) + size) {
		lseek(fd, off, SEEK_SET);
		len = read(fd, buf, sizeof(vertexid_t));
		if (len == 0)
			/* EOF reached */
			break;
		if (len != sizeof(vertexid_t))
			return (-1);
#if _DEBUG
		printf("vertex_write: read %lu bytes of vertex id\n", len);
#endif
		id = *((vertexid_t *) buf);
		if (id == v->id) {
			/*
			 * The vertex id is already on secondary storage
			 * so just "drop the head" and update the tuple
			 */

			/* Write the tuple buffer if there is one */
			if (size > 0) {
				len = write(fd, v->tuple->buf, size);
#if _DEBUG
				printf("vertex_write: ");
				printf("write %lu bytes of tuple buffer\n",
					len);
#endif
				return len;
			}
			return 0;
		}
	}
	/*
	 * The vertex id was not found in the search so "drop the head"
	 * and write the vertex id and tuple buffer.
	 */
	len = write(fd, &(v->id), sizeof(vertexid_t));
#if _DEBUG
	printf("vertex_write: wrote %lu bytes of vertex id\n", len);
#endif
	if (len != sizeof(vertexid_t))
		return (-1);

	/* Write the tuple buffer if there is one */
	if (size > 0) {
		write(fd, v->tuple->buf, size);
#if _DEBUG
		printf("vertex_write: ");
		printf("write %lu bytes of tuple buffer\n", len);
#endif
		return len;
	}
	return 0;
}
