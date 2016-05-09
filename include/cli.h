#ifndef __CLI_GRAPH_H
#define __CLI_GRAPH_H

#include "graph.h"

#define PROMPT		"db> "
#define BUFSIZE		4096
#define MAX_GRAPHS	8

/* Graphs */
extern struct graph graphs[];
extern int currgraph;

void nextarg(char *ln, int *pos, char *sep, char *arg);

void cli_about(char *cmdline);
void cli_help(char *cmdline);
void cli_graph(char *cmdline);

#endif
