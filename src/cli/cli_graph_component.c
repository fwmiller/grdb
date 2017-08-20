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
#include "graph.h"

int numbers_only(const char *s);

static int
cli_graph_next_cno()
{
	char s[BUFSIZE];
	DIR *dirfd;
	struct dirent *de;
	int n = (-1);
	int i = 0;

	/* Search the current graph directory for the highest cno */
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d", grdbdir, gno);
	if ((dirfd = opendir(s)) == NULL)
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
	printf("cli_graph_next_cno: next cno %d\n", n);
#endif
	return n;
}

static void
cli_graph_component_new()
{
	struct vertex v;
	char s[BUFSIZE];
	int fd, n;

	n = cli_graph_next_cno();
	if (n < 0) {
#if _DEBUG
		printf("cli_graph_component_new: bad next cno\n");
#endif
		return;
	}
	/* Create first vertex in component */
	vertex_init(&v);
	vertex_set_id(&v, 1);

	/* Persistence... */
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d", grdbdir, gno, n);
	mkdir(s, 0755);

	/* Create component vertex file */
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/v", grdbdir, gno, n);
#if _DEBUG
	printf("cli_graph_component_new: open vertex file %s\n", s);
#endif
	fd = open(s, O_RDWR | O_CREAT, 0644);
	if (fd < 0)
		return;

	/* Write first vertex tuple */
	vertex_write(&v, fd);
	close(fd);
}

void
cli_graph_component(char *cmdline, int *pos)
{
	char s[BUFSIZE];
	int fd;

	memset(s, 0, BUFSIZE);
	nextarg(cmdline, pos, " ", s);

	if (strcmp(s, "new") == 0 || strcmp(s, "n") == 0)
		cli_graph_component_new();

	else if (strlen(s) == 0) {
		char s[BUFSIZE];

		memset(s, 0, BUFSIZE);
		sprintf(s, "%d", gno);
		cli_components_print(s);
	}
}
