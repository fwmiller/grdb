#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "cli.h"
#include "graph.h"

void
cli_components_print(FILE *out, char *gname, int with_tuples)
{
	char s[BUFSIZE];
	DIR *dirfd;
	struct dirent *de;

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
			cli_component_print(
				out, gname, de->d_name, with_tuples);
		}
	}
	closedir(dirfd);
}

void
cli_graphs_print()
{
	DIR *dirfd;
	struct dirent *de;
	char s[BUFSIZE];
	FILE *out;
	char ch;
	int len;

	if ((dirfd = opendir(grdbdir)) == NULL)
		return;

	memset(s, 0, BUFSIZE);
	sprintf(s, "/tmp/grdbGraphs");
	out = fopen(s, "w");
	if (out == NULL) {
		printf("cli_graphs_print: fopen %s failed\n", s);
		return;
	}
	/*
	 * Loop over directories in grdb directory to display
	 * each graph
	 */
	for (;;) {
		de = readdir(dirfd);
		if (de == NULL)
			break;

		if (strcmp(de->d_name, ".") != 0 &&
		    strcmp(de->d_name, "..") != 0)
			/* no tuples */
			cli_components_print(out, de->d_name, 0);
	}
	closedir(dirfd);
	fclose(out);

	out = fopen(s, "r");
	if (out == NULL) {
		printf("cli_graphs_print: fopen %s failed\n", s);
		return;
	}
	for (;;) {
		len = fread(&ch, 1, 1, out);
		if (len <= 0)
			break;
		printf("%c", ch);
	}
	fclose(out);

	/* Remove the file */
	//unlink(s);
}
