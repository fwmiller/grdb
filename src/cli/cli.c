#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "graph.h"
#include "schema.h"

/* Graphs */
vertex_t graphs[MAX_GRAPHS];
int currgraph = 0;

char *readline(char *prompt);

struct cli_cmd {
	char *cmd;
	void (*f) (char *);
};

static struct cli_cmd cmds[] = {
	{ "about", cli_about },
	{ "help", cli_help },
	{ "g", cli_graph },
};

void
cli_about(char *cmdline)
{
	printf("Graph Database\n");
	printf("(C) Frank W. Miller\n");
}

void
cli_help(char *cmdline)
{
	int i, ncmds;

	ncmds = sizeof(cmds) / sizeof(struct cli_cmd);

	for (i = 0; i < ncmds; i++)
		printf("%s ", cmds[i].cmd);

	printf("(quit|q)\n");
}

void
cli()
{
	char *cmdline = NULL;
	char cmd[BUFSIZE];
	int i, ncmds, pos;

	cli_about(NULL);

	for (i = 0; i < MAX_GRAPHS; i++)
		graphs[i] = NULL;

	ncmds = sizeof(cmds) / sizeof(struct cli_cmd);

	for (;;) {
		if (cmdline != NULL) {
			free(cmdline);
			cmdline = NULL;
		}
		cmdline = readline(PROMPT);
		if (cmdline == NULL)
			continue;

		if (strlen(cmdline) == 0)
			continue;

		if (strcmp(cmdline, "?") == 0) {
			cli_help(cmdline);
			continue;
		}
		if (strcmp(cmdline, "quit") == 0 ||
		    strcmp(cmdline, "q") == 0)
			break;

		memset(cmd, 0, BUFSIZE);
		pos = 0;
		nextarg(cmdline, &pos, " ", cmd);

		for (i = 0; i < ncmds; i++)
			if (strcmp(cmd, cmds[i].cmd) == 0) {
				if (cmds[i].f != NULL)
					(*(cmds[i].f)) (cmdline);
				break;
			}
	}
}
