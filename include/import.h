#ifndef __IMPORT_H
#define __IMPORT_H


#include "graph.h"


struct ivertex {
	/* Every vertex has a unique vertex id */
	vertexid_t id;

	/* Every vertex can have a unique schema */
	schema_t s;

	/* Links for vertex set data structure in memory */
	struct vertex *prev;
	struct vertex *next;

	/* Vertex tuple */
	tuple_t tuple;
};

struct iedge {
	/* Every edge has two vertex ids */
	vertexid_t id1, id2;

	/* Every edge can have a unique schema */
	schema_t s;

	/* Links for edge set data structure in memory */
	struct edge *prev;
	struct edge *next;

	/* Edge tuple */
	tuple_t tuple;
};

struct igraph {
	struct vertex *v;	/* Vertex list */
	struct edge *e;		/* Edge list */

	/* Links for graph data structure in memory */
	struct graph *prev;
	struct graph *next;
};

typedef struct ivertex *ivertex_t;
typedef struct iedge *iedge_t;
typedef struct igraph *igraph_t;


int graph_import(igraph_t ig, graph_t g);


#endif
