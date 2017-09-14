#include <assert.h>
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
	ssize_t len;
	enum_list_t el = NULL;
	schema_t schema = NULL;
	char s[BUFSIZE];

	/* Load enums */
	efd = enum_file_open(grdbdir, gno, cno);
	if (efd < 0)
		return;

	enum_list_init(&el);
	enum_list_read(&el, efd);

	/* Load the schema */
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/%s",
		grdbdir, gno, cno, (st == VERTEX ? "sv" : "se"));
	fd = open(s, O_RDWR | O_CREAT, 0644);
	if (fd < 0)
		return;

	schema = schema_read(fd, el);
	close(fd);
	if (schema == NULL)
		new_schema_size = 0;
	else
		new_schema_size = schema_size(schema);
#if _DEBUG
	printf("cli_graph_update_tuples: ");
	printf("old schema size %d new schema size %d\n",
		old_schema_size, new_schema_size);
#endif
	if (new_schema_size - old_schema_size <= 0)
		return;

	/* Open vertex file for reading */
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/v", grdbdir, gno, cno);
	fd1 = open(s, O_RDONLY);

	/* Open new vertex file for writing */
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/v.tmp", grdbdir, gno, cno);
	fd2 = open(s, O_WRONLY | O_CREAT, 0644);

	/* Loop over all the vertices */
	for (;;) {
		/* Read vertex id and tuple */
		memset(s, 0, BUFSIZE);
		len = read(fd1, s, sizeof(vertexid_t) + old_schema_size);
		if (len < sizeof(vertexid_t) + old_schema_size) {
			if (len == 0)
				break;
			if (len < 0) {
				/* XXX Need to close fds */
				return;
			}
		}
		/* Append the attribute with a default value */
		memset(s + sizeof(vertexid_t) + old_schema_size, 0,
			new_schema_size - old_schema_size);

		/* Write the updated tuple to the new vertex file */
		write(fd2, s, sizeof(vertexid_t) + new_schema_size);
	}
	/* Close both files */
	close(fd1);
	close(fd2);

	/* Move the vertex to a temporary file name */

	/* Move the new vertex file to the proper vertex file name */

	/* Unlink the old vertex file from its temporary file name */
}
