#ifndef __CLI_GRAPH_H
#define __CLI_GRAPH_H

#include "graph.h"

#define PROMPT		"grdb> "
#define BUFSIZE		4096
#define MAX_GRAPHS	8

enum schema_type { VERTEX, EDGE };

typedef enum schema_type schema_type_t;

/* Graphs */
extern graph_t graphs, current;

void nextarg(char *ln, int *pos, char *sep, char *arg);

void cli_enum(char *cmdline, int *pos);
void cli_graph(char *cmdline, int *pos);

void cli_graphs_insert(graph_t g);

#endif
