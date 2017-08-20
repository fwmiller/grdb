#include <ctype.h>
#include <dirent.h>
#if _DEBUG
#include <errno.h>
#endif
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "cli.h"

int
numbers_only(const char *s)
{
	while (*s) {
		if (isdigit(*s++) == 0)
			return 0;
	}
	return 1;
}

static int
cli_graph_next_gno()
{
	DIR *dirfd;
	struct dirent *de;
	int n = (-1);
	int i = 0;

	/* Search the grdb directory for the highest gno */
	if ((dirfd = opendir(grdbdir)) == NULL)
		return (-1);

	for (;;) {
		de = readdir(dirfd);
		if (de == NULL)
			break;

		if (!numbers_only(de->d_name))
			continue;

		i = atoi(de->d_name);
		if (i > n)
			n = i;
	}
	closedir(dirfd);
	n++;
#if _DEBUG
	printf("cli_graph_next_gno: next gno %d\n", n);
#endif
	return n;
}

void
cli_graph_new(char *cmdline, int *pos)
{
	struct vertex v;
	char s[BUFSIZE];
	int fd, n;

	n = cli_graph_next_gno();
	if (n < 0) {
#if _DEBUG
		printf("cli_graph_new: bad next gno\n");
#endif
		return;
	}
	/* Initialize global graph and component numbers if necessary */
	if (gno < 0 && cno < 0) {
		gno = n;
		cno = 0;
	}
	/* Create first vertex in component */
	vertex_init(&v);
	vertex_set_id(&v, 1);

	/* Persistence... */
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d", grdbdir, n);
	mkdir(s, 0755);

	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d", grdbdir, n, 0);
	mkdir(s, 0755);

	/* Create component vertex file */
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/v", grdbdir, n, 0);
#if _DEBUG
	printf("cli_graph_new: open vertex file %s\n", s);
#endif
	fd = open(s, O_RDWR | O_CREAT, 0644);
	if (fd < 0)
		return;

	/* Write first vertex tuple */
	vertex_write(&v, fd);
	close(fd);
}
