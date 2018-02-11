#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "config.h"
#include "enum.h"

int
enum_list_union(
	enum_list_t el1,
	enum_list_t el2,
	char *grdbdir,
	int gno,
	int cidx)
{
	enum_t e;
	char s[BUFSIZE];
	int fd;

	assert (el1 != NULL && el2 != NULL);

        /* Open file for concatenating the two enums */
        memset(s, 0, BUFSIZE);
        sprintf(s, "%s/%d/%d/enum", grdbdir, gno, cidx);
	fd = open(s, O_WRONLY | O_CREAT, 0644);
	if (fd < 0) {
#if _DEBUG
		printf("enum_list_union: open %s failed\n", s);
#endif
		return (-1);
	}
	/* Concatenate the enum lists */
	for (e = el1; e->next != NULL; e = e->next);
	e->next = el2;

	/* Write the concatenated enum list to the file */
	enum_list_write(el1, fd);

	close(fd);
	return 0;
}
