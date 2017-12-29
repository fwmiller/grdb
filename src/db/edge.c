#include <assert.h>
#if _DEBUG
#include <errno.h>
#endif
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
edge_file_init(int gidx, int cidx)
{
	char s[BUFSIZE];
	int fd;

	/* Create component edge file */
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/e", GRDBDIR, gidx, cidx);
#if _DEBUG
	printf("edge_file_init: open edge file %s\n", s);
#endif
	fd = open(s, O_RDWR | O_CREAT, 0644);
#if _DEBUG
	if (fd < 0) {
		printf("edge_file_init: open edge file failed (%s)\n",
			strerror(errno));
	}
#endif
	return fd;
}

void
edge_set_vertices(edge_t e, vertexid_t id1, vertexid_t id2)
{
	assert(e != NULL);

	e->id1 = id1;
	e->id2 = id2;
}

void
edge_print(FILE *out, edge_t e)
{
	assert(e != NULL);

	//printf("(%04llx,%04llx)", e->id1, e->id2);
	fprintf(out, "(%llu,%llu)", e->id1, e->id2);
}
