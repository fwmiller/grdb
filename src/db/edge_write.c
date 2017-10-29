#include <assert.h>
#if _DEBUG
#include <stdio.h>
#endif
#include <stdlib.h>
#include <unistd.h>
#include "graph.h"

/*
 * Write the tuple record to secondary storage.  The record contains two
 * 64-bit vertex ids followed by the edge tuple.  Assume the ids and tuple
 * are set to be written.
 */
ssize_t
edge_write(edge_t e, int fd)
{
	off_t off;
	ssize_t len, size;
	vertexid_t id1, id2;
	char buf[sizeof(vertexid_t) << 1];

	assert(e != NULL);
#if _DEBUG
	printf("edge_write: write edge (%llu,%llu)\n", e->id1, e->id2);
#endif
	if (e->tuple == NULL)
		size = 0;
	else
		size = schema_size(e->tuple->s);
#if _DEBUG
	printf("edge_write: schema size = %lu bytes\n", size);
#endif

	/* Search for edge ids in current component */
	for (off = 0;; off += (sizeof(vertexid_t) << 1) + size) {
		lseek(fd, off, SEEK_SET);
		len = read(fd, buf, sizeof(vertexid_t) << 1);
#if _DEBUG
		printf("edge_write: read %lu bytes of edge ids\n", len);
#endif
		if (len == 0)
			/* EOF reached */
			break;

		if (len != sizeof(vertexid_t) << 1)
			return (-1);

		id1 = *((vertexid_t *) buf);
		id2 = *((vertexid_t *) (buf + sizeof(vertexid_t)));
		if (id1 == e->id1 && id2 == e->id2) {
			/*
			 * The edge id is already on secondary storage
			 * so just "drop the head" and update the tuple
			 */
			if (size > 0) {
				len = write(fd, e->tuple->buf, size);
#if _DEBUG
				printf("edge_write: ");
				printf("write %lu bytes to tuple buffer\n",
					len);
#endif
				return len;
			}
			return 0;
		}
	}
	/*
	 * The edge ids were not found in the search so "drop the head"
	 * and write the edge ids and tuple buffer.
	 */
	len = write(fd, &(e->id1), sizeof(vertexid_t));
#if _DEBUG
	printf("edge_write: wrote %lu bytes of vertex id\n", len);
#endif
	if (len != sizeof(vertexid_t))
		return (-1);

	len = write(fd, &(e->id2), sizeof(vertexid_t));
#if _DEBUG
	printf("edge_write: wrote %lu bytes of vertex id\n", len);
#endif
	if (len != sizeof(vertexid_t))
		return (-1);

	if (size > 0) {
		len = write(fd, e->tuple->buf, size);
#if _DEBUG
		printf("edge_write: write %lu bytes to tuple buffer\n", len);
#endif
		return len;
	}
	return 0;
}
