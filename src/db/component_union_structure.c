#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include "config.h"

int
component_union_structures(
	int cidx1,
	int cidx2,
	char *grdbdir,
	int gno,
	int cidx)
{
	int fd, fdnew;
	char s[BUFSIZE];

	/* Open new vertex file for unioned component */
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d", grdbdir, gno, cidx);
	fdnew = open(s, O_WRONLY | O_CREAT, 0644);
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
	/* Copy all the vertices to the new vertex file */

	/* Open second input component vertex file */

	/* Concatenate all the vertices to the end of new vertex file */


	/* XXX Need to do duplicate elimination on the new vertex file */


	return (-1);
}
