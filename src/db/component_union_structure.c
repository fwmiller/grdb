#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "config.h"
#include "graph.h"

int
component_union_structures(
	int cidx1,
	int cidx2,
	char *grdbdir,
	int gno,
	int cidx)
{
	enum_list_t el, el1, el2;
	schema_t sv, sv1, sv2, se, se1, se2;
	int fd, fdnew;
	char s[BUFSIZE];
	char *buf;
	int len, size;

	/*
	 * Load the enums for the two input components and the output
	 * component
	 */
	enum_list_init(&el);
	enum_list_init(&el1);
	enum_list_init(&el2);

	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/enum", grdbdir, gno, cidx1);
	fd = open(s, O_RDONLY);
	if (fd >= 0) {
		enum_list_read(&el1, fd);
		close(fd);
	}
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/enum", grdbdir, gno, cidx2);
	fd = open(s, O_RDONLY);
	if (fd >= 0) {
		enum_list_read(&el2, fd);
		close(fd);
	}
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/enum", grdbdir, gno, cidx);
	fd = open(s, O_RDONLY);
	if (fd >= 0) {
		enum_list_read(&el, fd);
		close(fd);
	}
	/*
	 * Load the schemas for the two input components and the output
	 * component
	 */
	schema_init(&sv);
	schema_init(&sv1);
	schema_init(&sv1);
	schema_init(&se);
	schema_init(&se1);
	schema_init(&se1);

	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/sv", grdbdir, gno, cidx1);
	fd = open(s, O_RDONLY);
	if (fd >= 0) {
		sv1 = schema_read(fd, el1);
		close(fd);
	}
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/sv", grdbdir, gno, cidx2);
	fd = open(s, O_RDONLY);
	if (fd >= 0) {
		sv2 = schema_read(fd, el2);
		close(fd);
	}
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/sv", grdbdir, gno, cidx);
	fd = open(s, O_RDONLY);
	if (fd >= 0) {
		sv = schema_read(fd, el);
		close(fd);
	}
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/se", grdbdir, gno, cidx1);
	fd = open(s, O_RDONLY);
	if (fd >= 0) {
		se1 = schema_read(fd, el1);
		close(fd);
	}
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/se", grdbdir, gno, cidx2);
	fd = open(s, O_RDONLY);
	if (fd >= 0) {
		se2 = schema_read(fd, el2);
		close(fd);
	}
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/se", grdbdir, gno, cidx);
	fd = open(s, O_RDONLY);
	if (fd >= 0) {
		se = schema_read(fd, el);
		close(fd);
	}
	/*
	 * Union the graph structures and tuple data for the two input
	 * components into the output component
	 */

	/* Open new vertex file for unioned component */
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d", grdbdir, gno, cidx);
	fdnew = open(s, O_RDWR | O_CREAT, 0644);
	if (fdnew < 0) {
#if _DEBUG
		printf("component_union_structures: open %s failed\n", s);
#endif
		return (-1);
	}
	/* Open first input component vertex file */
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/v", grdbdir, gno, cidx1);
	fd = open(s, O_RDONLY);
	if (fd < 0) {
#if _DEBUG
		printf("component_union_structures: open %s failed\n", s);
#endif
		return (-1);
	}
	/* Add all vertices and their tuple data to the new vertex file */
	size = schema_size(sv1);
	buf = malloc(sizeof(vertexid_t) + size);
	assert (buf != NULL);
	for (;;) {
		memset(buf, 0, sizeof(vertexid_t) + size);
		len = read(fd, buf, sizeof(vertexid_t) + size);
		if (len == 0)
			break;

		vertex_write_from(
			*((vertexid_t *) buf),
			buf + sizeof(vertexid_t),
			sv1, fdnew, sv);
	}
#if 0
	free(buf);
#endif
	close(fd);


	/* Open second input component vertex file */
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/v", grdbdir, gno, cidx2);
	fd = open(s, O_RDONLY);
	if (fd < 0) {
#if _DEBUG
		printf("component_union_structures: open %s failed\n", s);
#endif
		return (-1);
	}
	/* Add all vertices and their tuple data to the new vertex file */
	for (;;) {
		/* Read vertex data from old file */

		/*
		 * Write the vertex data into the proper fields of the
		 * different schema in the new file
		 */
	}
	close(fd);

	return (-1);
}
