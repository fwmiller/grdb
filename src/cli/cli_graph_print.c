#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "cli.h"
#include "graph.h"

void
cli_components_print(char *gname, int with_tuples)
{
	char s[BUFSIZE];
	DIR *dirfd;
	struct dirent *de;
	struct component c;
	int fd, gidx, cidx;

	/* Loop over directories in graph to display each component */
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%s", grdbdir, gname);
	if ((dirfd = opendir(s)) == NULL)
		return;

	for (;;) {
		de = readdir(dirfd);
		if (de == NULL)
			break;

		if (strcmp(de->d_name, ".") != 0 &&
		    strcmp(de->d_name, "..") != 0) {
			gidx = atoi(gname);
			cidx = atoi(de->d_name);

			if (gidx == gno && cidx == cno)
				printf(">");

			printf("%s.%s:", gname, de->d_name);

			component_init(&c);

			/* Load enums */
			c.efd = enum_file_open(grdbdir, gidx, cidx);
			if (c.efd >= 0) {
				enum_list_init(&(c.el));
				enum_list_read(&(c.el), c.efd);
			}
			/* Load vertex schema */
			memset(s, 0, BUFSIZE);
			sprintf(s, "%s/%d/%d/sv", grdbdir, gidx, cidx);
			fd = open(s, O_RDONLY);
			if (fd >= 0) {
				c.sv = schema_read(fd, c.el);
				close(fd);
			}

			/* Load edge schema */
			memset(s, 0, BUFSIZE);
			sprintf(s, "%s/%d/%d/se", grdbdir, gidx, cidx);
			fd = open(s, O_RDONLY);
			if (fd >= 0) {
				c.se = schema_read(fd, c.el);
				close(fd);
			}

			/* Open vertex file */
			memset(s, 0, BUFSIZE);
			sprintf(s, "%s/%s/%s/v", grdbdir, gname, de->d_name);
			c.vfd = open(s, O_RDWR);

			/* Open edge file */
			memset(s, 0, BUFSIZE);
			sprintf(s, "%s/%s/%s/e", grdbdir, gname, de->d_name);
			c.efd = open(s, O_RDWR);

			component_print(&c, with_tuples);
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

	/* Loop over directories in grdb directory to display each graph */
	if ((dirfd = opendir(grdbdir)) == NULL)
		return;

	for (;;) {
		de = readdir(dirfd);
		if (de == NULL)
			break;

		if (strcmp(de->d_name, ".") != 0 &&
		    strcmp(de->d_name, "..") != 0) {
			cli_components_print(de->d_name, 0); /* no tuples */
		}
	}
	closedir(dirfd);
}
