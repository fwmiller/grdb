#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "cli.h"
#include "graph.h"

void
cli_component_print(
	FILE *out, char *gname, char *cname, int with_tuples)
{
	struct component c;
	int gidx, cidx;
	int fd;
	char s[BUFSIZE];

	gidx = atoi(gname);
	cidx = atoi(cname);
	if (gidx == gno && cidx == cno)
		fprintf(out, ">");

	fprintf(out, "%s.%s:", gname, cname);

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
	sprintf(s, "%s/%s/%s/v", grdbdir, gname, cname);
	c.vfd = open(s, O_RDWR);

	/* Open edge file */
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%s/%s/e", grdbdir, gname, cname);
	c.efd = open(s, O_RDWR);

	component_print(out, &c, with_tuples);
	fprintf(out, "\n");

	/* Close files */
	close(c.efd);
	close(c.vfd);
}
