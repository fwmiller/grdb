#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "cli.h"
#include "graph.h"

/* Graphs */
graph_t graphs = NULL;
graph_t current = NULL;

char *readline(char *prompt);

static int tty = 0;

static void
cli_about()
{
	printf("Graph Database\n");
	printf("(C) Frank W. Miller\n");
#if 0
	printf("This program comes with ABSOLUTELY NO WARRANTY\n");
	printf(
	 "This is free software. You are welcome to redistribute it under\n");
	printf("the terms of the GNU General Public License Version 3\n");
#endif
}

static void
cli_help()
{
	return;
}

int
graphs_get_current_index()
{
	graph_t g;
	int cnt;

	for (cnt = 0, g = graphs; g != NULL; cnt++, g = g->next)
		if (g == current)
			return cnt;
	return (-1);
}

void
cli()
{
	char *cmdline = NULL;
	char cmd[BUFSIZE], prompt[BUFSIZE];
	int pos;

	tty = isatty(STDIN_FILENO);
	if (tty)
		cli_about();

	for (;;) {
		if (cmdline != NULL) {
			free(cmdline);
			cmdline = NULL;
		}


		// cmdline = readline(PROMPT);
		memset(prompt, 0, BUFSIZE);
		if (graphs == NULL)
			//sprintf(prompt, "> ");
			sprintf(prompt, "grdb> ");
		else
			//sprintf(prompt, "%d> ", graphs_get_current_index());
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

void cli_graphs_insert(graph_t g)
{
	graph_t f;

	if (graphs == NULL) {
		graphs = g;
		return;
	}
	for (f = graphs; f->next != NULL; f = f->next);
	f->next = g;
}
