#ifndef __CLI_GRAPH_H
#define __CLI_GRAPH_H

#include "graph.h"
#include "schema.h"

#define PROMPT		"grdb> "
#define BUFSIZE		(1 << 12)
#define MAX_GRAPHS	8

extern char grdbdir[];

/* Current graph and component numbers */
extern int gno, cno;

void nextarg(char *ln, int *pos, char *sep, char *arg);

int graphs_get_index(graph_t g);
int components_get_index(graph_t g, component_t c);

void cli_component_print(
	FILE *out, char *gname, char *cname, int with_tuples);
void cli_components_print(FILE *out, char *gname, int with_tuples);
void cli_graphs_print();

void cli_graph_enum(char *cmdline, int *pos);
void cli_graph(char *cmdline, int *pos);


#endif
