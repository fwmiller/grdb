#include <assert.h>
#if _DEBUG
#include <errno.h>
#endif
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
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

/*
 * The vertex file is arranged as a packed list of vertex records.  Each
 * record contains a 64-bit vertex id followed by the vertex tuple.  If
 * an error or some sort occurs, the value (-1) is returned.  The value
 * zero means the end-of-file was reached.  Otherwise, the number of bytes
 * read in for the vertex tuple are returned.
 */
ssize_t
vertex_read(vertex_t v, schema_t schema, int fd)
{
	off_t off;
	ssize_t len, size;
	vertexid_t id;
	char buf[sizeof(vertexid_t)];

	assert(v != NULL);
#if _DEBUG
	printf("vertex_read: read vertex %llu\n", v->id);
#endif
	if (schema == NULL)
		size = 0;
	else
		size = schema_size(schema);
#if _DEBUG
	printf("vertex_read: schema size = %lu bytes\n", size);
#endif
	/* Search for vertex id in current component */
	for (off = 0;;) {
		lseek(fd, off, SEEK_SET);
		len = read(fd, buf, sizeof(vertexid_t));
		if (len != sizeof(vertexid_t)) {
#if _DEBUG
			printf("vertex_read: ");
			printf("read %lu bytes of vertex id\n", len);
#endif
			return (-1);
		}
		off += sizeof(vertexid_t);

		id = *((vertexid_t *) buf);

		/* Read tuple buffer if there is one */
		if (size > 0) {
			if (v->tuple == NULL)
				tuple_init(&(v->tuple), schema);
			memset(v->tuple->buf, 0, size);
			lseek(fd, off, SEEK_SET);
			len = read(fd, v->tuple->buf, size);
#if _DEBUG
			printf("vertex_read: ");
			printf("read %lu bytes to tuple buffer\n", len);
#endif
			off += size;
		}
		if (id == v->id)
			return len;
	}
	return 0;
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
