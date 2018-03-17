#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "config.h"
#include "schema.h"

int
schema_union(
	schema_type_t st,
	schema_t s1,
	schema_t s2,
	char *grdbdir,
	int gno,
	int cidx)
{
	char s[BUFSIZE];
	int fd;

	/* Open file for concatenating the two schemas */
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/%s",
		grdbdir, gno, cidx, (st == VERTEX ? "sv" : "se"));
	fd = open(s, O_WRONLY | O_CREAT, 0644);
	if (fd < 0) {
#if _DEBUG
		printf("schema_union: open %s failed\n", s);
#endif
		return (-1);
	}
	/* Concatentate the schemas in the new file */
	if (s1 != NULL)
		schema_write(s1, fd, 0);
	if (s2 != NULL)
		schema_write(s2, fd, 1);

	close(fd);

	/* XXX Need to do duplicate elimination */

	return 0;
}
