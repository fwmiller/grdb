#include <assert.h>
#if _DEBUG
#include <errno.h>
#endif
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "cli.h"
#include "graph.h"

#if _DEBUG
void bufdump(char *buf, int size);
#endif

void
cli_graph_update_tuples(schema_type_t st, int old_schema_size)
{
	int efd, fd, fd1, fd2;
	int new_schema_size;
	int readlen, writelen;
	ssize_t len;
	enum_list_t el = NULL;
	schema_t schema = NULL;
	attribute_t attr = NULL;
	char s[BUFSIZE], s1[BUFSIZE];

	/* Load enums */
	efd = enum_file_open(grdbdir, gno, cno);
	if (efd < 0)
		return;

	enum_list_init(&el);
	enum_list_read(&el, efd);
	close(efd);

	/* Load the schema */
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/%s",
		grdbdir, gno, cno, (st == VERTEX ? "sv" : "se"));
	fd = open(s, O_RDONLY | O_CREAT, 0644);
	if (fd < 0)
		return;

	schema = schema_read(fd, el);
	close(fd);
	if (schema == NULL)
		new_schema_size = 0;
	else
		new_schema_size = schema_size(schema);

	attr = schema_attribute_last(schema);

#if _DEBUG
	printf("cli_graph_update_tuples: ");
	printf("old schema size %d new schema size %d\n",
		old_schema_size, new_schema_size);
#endif
	if (new_schema_size - old_schema_size <= 0)
		return;

	/* Open vertex/edge file for reading */
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/%s",
		grdbdir, gno, cno, (st == VERTEX ? "v" : "e"));
#if _DEBUG
	printf("cli_graph_update_tuples: ");
	printf("current vertex file %s\n", s);
#endif
	fd1 = open(s, O_RDONLY);

	/* Open new vertex/edge file for writing */
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/%s.new",
		grdbdir, gno, cno, (st == VERTEX ? "v" : "e"));
#if _DEBUG
	printf("cli_graph_update_tuples: ");
	printf("new vertex file %s\n", s);
#endif
	fd2 = open(s, O_WRONLY | O_CREAT, 0644);

	/* Loop over all the vertices */
	for (;;) {
		/* Read vertex id(s) and tuple */
		memset(s, 0, BUFSIZE);

		readlen = old_schema_size +
			(st == VERTEX ?
				sizeof(vertexid_t) :
				(sizeof(vertexid_t) << 1));

		len = read(fd1, s, readlen);
		if (len < readlen) {
			if (len == 0)
				break;
			if (len < 0) {
				/* XXX Need to close fds */
				return;
			}
		}
#if _DEBUG
		if (st == VERTEX) {
			printf("cli_graph_update_tuples: ");
			printf("update vertex %llu\n", *((vertexid_t *) s));
		} else {
			printf("cli_graph_update_tuples: ");
			printf("update edge (%llu,%llu)\n",
				*((vertexid_t *) s),
				*((vertexid_t *) (s + sizeof(vertexid_t))));
		}
#endif
		/*
		 * Set the default value for the new attribute based on
		 * its base type
		 */
		attr = schema_attribute_last(schema);
		tuple_set_default_value(
			attr->bt,
			s + (st == VERTEX ?
				sizeof(vertexid_t) :
				(sizeof(vertexid_t) << 1)),
			old_schema_size);

		/*
		 * Write the updated tuple to the new vertex file.  This
		 * means picking up additional buffer space to be written
		 * for this tuple.
		 */
		writelen = new_schema_size +
			(st == VERTEX ?
				sizeof(vertexid_t) :
				(sizeof(vertexid_t) << 1));
#if _DEBUG
		printf("cli_graph_update_tuples: ");
		printf("writelen %d\n", writelen);
#endif
		write(fd2, s, writelen);
	}
	/* Close both files */
	close(fd1);
	close(fd2);

	/* Move the vertex to a temporary file name */
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/%s",
		grdbdir, gno, cno, (st == VERTEX ? "v" : "e"));
	memset(s1, 0, BUFSIZE);
	sprintf(s1, "%s/%d/%d/%s.tmp",
		grdbdir, gno, cno, (st == VERTEX ? "v" : "e"));
	if (rename(s, s1) < 0) {
#if _DEBUG
		printf("cli_graph_update_tuples: ");
		printf("rename %s to %s failed (%s)\n",
			s, s1, strerror(errno));
#endif
	}

	/* Move the new vertex file to the proper vertex file name */
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/%s.new",
		grdbdir, gno, cno, (st == VERTEX ? "v" : "e"));
	memset(s1, 0, BUFSIZE);
	sprintf(s1, "%s/%d/%d/%s",
		grdbdir, gno, cno, (st == VERTEX ? "v" : "e"));
	rename(s, s1);
	if (rename(s, s1) < 0) {
#if _DEBUG
		printf("cli_graph_update_tuples: ");
		printf("rename %s to %s failed (%s)\n",
			s, s1, strerror(errno));
#endif
	}
	/* XXX Unlink the old vertex file from its temporary file name */
}
