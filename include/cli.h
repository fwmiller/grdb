#ifndef __CLI_GRAPH_H
#define __CLI_GRAPH_H

#include "graph.h"

#define PROMPT		"grdb> "
#define BUFSIZE		(1 << 12)
#define MAX_GRAPHS	8

enum schema_type { VERTEX, EDGE };

typedef enum schema_type schema_type_t;

/* Graphs */
extern graph_t graphs, current_graph;
extern component_t current_component;

void nextarg(char *ln, int *pos, char *sep, char *arg);

int graphs_get_current_index();
int components_get_index(graph_t g);

void cli_enum(char *cmdline, int *pos);
void cli_graph(char *cmdline, int *pos);

void cli_components_print(graph_t g, int gno);


#endif
