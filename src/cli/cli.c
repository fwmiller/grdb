#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "cli.h"
#include "graph.h"

/* Home directory for user */
const char *homedir;
char grdbdir[BUFSIZE];

/* Graphs */
graph_t graphs = NULL;
graph_t current_graph = NULL;
component_t current_component = NULL;

char *readline(char *prompt);

static int tty = 0;

static void
cli_about()
{
	printf("Graph Database\n");
	printf("(C) Frank W. Miller\n");
}

static void
cli_help()
{
	return;
}

int
graphs_get_index(graph_t g)
{
	graph_t h;
	int cnt;

	for (cnt = 0, h = graphs; h != NULL; cnt++, h = h->next)
		if (h == g)
			return cnt;
	return (-1);
}

int
components_get_index(graph_t g, component_t c)
{
	component_t d;
	int cnt;

	for (cnt = 0, d = g->c; d != NULL; cnt++, d = d->next)
		if (d == c)
			return cnt;
	return (-1);
}

void
cli()
{
	char *cmdline = NULL;
	char cmd[BUFSIZE], prompt[BUFSIZE];
	int pos;
	struct stat sb;

	tty = isatty(STDIN_FILENO);
	if (tty)
		cli_about();


	/* Setup to load databases */
	homedir = getenv("HOME");
	memset(grdbdir, 0, BUFSIZE);
	strcpy(grdbdir, homedir);
	strcat(grdbdir, "/.grdb");
#if _DEBUG
	printf("grdb directory: %s\n", grdbdir);
#endif
	/* Check for existence of grdb directory */
	if (stat(grdbdir, &sb) != 0 || !S_ISDIR(sb.st_mode)) {
#if _DEBUG
		printf("creating grdb directory\n");
#endif
		mkdir(grdbdir, 0755);
	}

	/* Main command line loop */
	for (;;) {
		if (cmdline != NULL) {
			free(cmdline);
			cmdline = NULL;
		}
		// cmdline = readline(PROMPT);
		memset(prompt, 0, BUFSIZE);
		sprintf(prompt, "grdb> ");

		if (tty)
			cmdline = readline(prompt);
		else
			cmdline = readline("");

		if (cmdline == NULL)
			continue;

		if (strlen(cmdline) == 0)
			continue;

		if (!tty)
			printf("%s\n", cmdline);

		if (strcmp(cmdline, "?") == 0) {
			cli_help();
			continue;
		}
		if (strcmp(cmdline, "quit") == 0 ||
		    strcmp(cmdline, "q") == 0)
			break;

		memset(cmd, 0, BUFSIZE);
		pos = 0;
		nextarg(cmdline, &pos, " ", cmd);

		if (strcmp(cmd, "about") == 0 || strcmp(cmd, "a") == 0) {
			cli_about();
			continue;

		} else if (strcmp(cmd, "help") == 0 ||
			   strcmp(cmd, "h") == 0) {
			cli_help();
			continue;

		} else if (strcmp(cmd, "enum") == 0 ||
			   strcmp(cmd, "e") == 0) {
			cli_enum(cmdline, &pos);
			continue;

		} else if (strcmp(cmd, "graph") == 0 ||
			   strcmp(cmd, "g") == 0) {
			cli_graph(cmdline, &pos);
			continue;
		}
	}
}
