#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "config.h"
#include "graph.h"

void
edge_init(edge_t e)
{
	assert(e != NULL);
	memset(e, 0, sizeof(struct edge));
}

/* Initialize the edge file */
int
edge_file_init(component_t c, int gidx, int cidx)
{
	char s[BUFSIZE];

	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d", GRDBDIR, gidx);
	mkdir(s, 0755);

	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d", GRDBDIR, gidx, cidx);
	mkdir(s, 0755);

	/* Create component edge file */
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/e", GRDBDIR, gidx, cidx);
#if _DEBUG
	printf("edge_file_init: open edge file %s\n", s);
#endif
	c->efd = open(s, O_RDWR | O_CREAT, 0644);
	if (c->efd < 0) {
#if _DEBUG
		printf("edge_file_init: open edge file failed (%s)\n",
			strerror(errno));
#endif
		return c->efd;
	}
	return 0;
}

void
edge_set_vertices(edge_t e, vertexid_t id1, vertexid_t id2)
{
	assert(e != NULL);

	e->id1 = id1;
	e->id2 = id2;
}

void
edge_print(edge_t e)
{
	assert(e != NULL);

	//printf("(%04llx,%04llx)", e->id1, e->id2);
	printf("(%llu,%llu)", e->id1, e->id2);
}

/*
 * The edge file is arranged as a packed list of edge records.  Each
 * record contains two 64-bit vertex ids followed by the edge tuple.  If
 * an error or some sort occurs, the value (-1) is returned.  The value
 * zero means the end-of-file was reached.  Otherwise, the number of bytes
 * read in for the vertex tuple are returned.
 */
ssize_t
edge_read(edge_t e, int fd)
{
	off_t off;
	ssize_t len, size;
	vertexid_t id1, id2;
	char buf[sizeof(vertexid_t) << 1];

	assert(e != NULL);
	assert(e->tuple != NULL);
#if _DEBUG
	printf("edge_read: read edge (%llu,%llu)\n", e->id1, e->id2);
#endif
	size = schema_size(e->tuple->s);
#if _DEBUG
	printf("edge_read: schema size = %lu bytes\n", size);
#endif
	/* Search for edge in current component */
	for (off = 0;; off += (sizeof(vertexid_t) << 1) + size) {
		lseek(fd, off, SEEK_SET);
		len = read(fd, buf, sizeof(vertexid_t) << 1);
		if (len != sizeof(vertexid_t) << 1) {
#if _DEBUG
			printf("edge_read: ");
			printf("read %lu bytes to tuple buffer\n", len);
#endif
			return (-1);
		}
		id1 = *((vertexid_t *) buf);
		id2 = *((vertexid_t *) (buf + sizeof(vertexid_t)));
		if (id1 == e->id1 && id2 == e->id2) {
			memset(e->tuple->buf, 0, size);
			len = read(fd, e->tuple->buf, size);
#if _DEBUG
			printf("edge_read: ");
			printf("read %lu bytes to tuple buffer\n", len);
#endif
			return len;
		}
	}
	return 0;
}

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
				memset(e->tuple->buf, 0, size);
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
