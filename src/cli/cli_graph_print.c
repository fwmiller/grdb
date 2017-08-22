#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "cli.h"
#include "graph.h"

void
cli_components_print(char *gname)
{
	char s[BUFSIZE];
	DIR *dirfd;
	struct dirent *de;
	struct component c;

	/*
	 * Loop over directories in the graph to display each
	 * component
	 */
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%s", grdbdir, gname);
#if 0
	printf("cli_components_print: directory %s\n", s);
#endif
	if ((dirfd = opendir(s)) == NULL)
		return;

	for (;;) {
		de = readdir(dirfd);
		if (de == NULL)
			break;

		if (strcmp(de->d_name, ".") != 0 &&
		    strcmp(de->d_name, "..") != 0) {
#if 0
			printf("cli_components_print: ");
			printf("component %s\n", de->d_name);
#endif
			printf("%s.%s:", gname, de->d_name);

			component_init(&c);

			/* XXX Load vertex schema */

			/* XXX Load edge schema */

			/* XXX Load enums schema if any */

			/* Open vertex file */
			memset(s, 0, BUFSIZE);
			sprintf(s, "%s/%s/%s/v", grdbdir, gname, de->d_name);
			c.vfd = open(s, O_RDWR);

			/* Open edge file */
			memset(s, 0, BUFSIZE);
			sprintf(s, "%s/%s/%s/e", grdbdir, gname, de->d_name);
			c.efd = open(s, O_RDWR);

			component_print(&c, 0); /* no tuples */
			printf("\n");

			/* Close files */
			close(c.efd);
			close(c.vfd);
		}
	}
	closedir(dirfd);
}

void
cli_graphs_print()
{
	DIR *dirfd;
	struct dirent *de;

	/*
	 * Loop over directories in the grdb directory to display each
	 * graph
	 */
#if 0
	printf("cli_graphs_print: directory %s\n", grdbdir);
#endif
	if ((dirfd = opendir(grdbdir)) == NULL)
		return;

	for (;;) {
		de = readdir(dirfd);
		if (de == NULL)
			break;

		if (strcmp(de->d_name, ".") != 0 &&
		    strcmp(de->d_name, "..") != 0) {
#if 0
			printf("cli_graphs_print: graph %s\n", de->d_name);
#endif
			cli_components_print(de->d_name);
		}
	}
	closedir(dirfd);
}
