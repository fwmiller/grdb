#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "config.h"
#include "graph.h"

int numbers_only(const char *s);

void
graph_init(graph_t g)
{
	assert (g != NULL);
	memset(g, 0, sizeof(struct graph));
}

void
graph_file_init(int gidx)
{
	char s[BUFSIZE];

	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d", GRDBDIR, gidx);
	mkdir(s, 0755);
}

int
graph_next_cno(char *grdbdir, int gno)
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
	printf("graph_next_cno: next cno %d\n", n);
#endif
	return n;
}

void
graph_insert_component(graph_t g, component_t c)
{
	component_t d;

	if (g->c == NULL) {
		g->c = c;
		return;
	}
	for (d = g->c; d->next != NULL; d = d->next);
	d->next = c;
}
