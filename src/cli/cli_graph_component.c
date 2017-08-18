#if _DEBUG
#include <errno.h>
#endif
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "cli.h"
#include "graph.h"

void
cli_graph_component(char *cmdline, int *pos)
{
	char s[BUFSIZE];
	int fd;

	memset(s, 0, BUFSIZE);
	nextarg(cmdline, pos, " ", s);

	if (strcmp(s, "new") == 0 || strcmp(s, "n") == 0) {
		struct vertex v;

		/* Create first vertex in component */
		vertex_init(&v);
		vertex_set_id(&v, 1);

		/* Persistence... */
		memset(s, 0, BUFSIZE);
		sprintf(s, "%s/%d/%d", grdbdir, gno, cno);
		mkdir(s, 0755);

		/* Create component vertex file */
		memset(s, 0, BUFSIZE);
		sprintf(s, "%s/%d/%d/v", grdbdir, gno, cno);
#if _DEBUG
		printf("cli_graph_component: open vertex file %s\n", s);
#endif
		fd = open(s, O_RDWR | O_CREAT, 0644);
		if (fd < 0) {
#if _DEBUG
			printf("cli_graph_component: ");
			printf("open vertex file failed (%s)\n",
				strerror(errno));
#endif
			return;
		}
		/* Write first vertex tuple */
		vertex_write(&v, fd);

		close(fd);

	} else if (strlen(s) == 0) {
		char s[BUFSIZE];

		memset(s, 0, BUFSIZE);
		sprintf(s, "%d", gno);
		cli_components_print(s);
	}
}
