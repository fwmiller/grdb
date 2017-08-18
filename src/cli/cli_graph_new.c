#if _DEBUG
#include <errno.h>
#endif
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "cli.h"

void
cli_graph_new(char *cmdline, int *pos)
{
	struct vertex v;
	char s[BUFSIZE];
	int fd;

	/* Create first vertex in component */
	vertex_init(&v);
	vertex_set_id(&v, 1);

	/* Persistence... */
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d", grdbdir, gno);
	mkdir(s, 0755);

	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d", grdbdir, gno, cno);
	mkdir(s, 0755);

	/* Create component vertex file */
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/v", grdbdir, cno, cno);
#if _DEBUG
	printf("cli_graph_new: open vertex file %s\n", s);
#endif
	fd = open(s, O_RDWR | O_CREAT, 0644);
	if (fd < 0) {
#if _DEBUG
		printf("cli_graph_new: open vertex file failed (%s)\n",
			strerror(errno));
#endif
		return;
	}
	/* Write first vertex tuple */
	vertex_write(&v, fd);
	close(fd);
}
