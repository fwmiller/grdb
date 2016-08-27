#ifndef __GRAPH_H
#define __GRAPH_H


#include "schema.h"
#include "tuple.h"
#include "vertexid.h"


struct vertex {
	/* Every vertex has a unique vertex id */
	vertexid_t id;

	/* Links for vertex set data structure in memory */
	struct vertex *prev;
	struct vertex *next;

	/* Vertex tuple */
	tuple_t tuple;
};

struct edge {
	/* Every edge has two vertex ids */
	vertexid_t id1, id2;

	/* Links for edge set data structure in memory */
	struct edge *prev;
	struct edge *next;

	/* Edge tuple */
	tuple_t tuple;
};

struct graph {
	struct vertex *v;	/* Vertices */
	schema_t sv;		/* Vertex schema */
	struct edge *e;		/* Edges */
	schema_t se;		/* Edge schema */

	struct graph *next;
};

typedef struct vertex *vertex_t;
typedef struct edge *edge_t;
typedef struct graph *graph_t;


void vertex_init(vertex_t v);
void vertex_set_id(vertex_t v, vertexid_t id);
void vertex_print(vertex_t v);

void edge_init(edge_t e);
void edge_set_vertices(edge_t e, vertexid_t id1, vertexid_t id2);
void edge_print(edge_t e);

void graph_init(graph_t g);
void graph_insert_vertex(graph_t g, vertex_t v);
vertex_t graph_find_vertex_by_id(graph_t g, vertexid_t id);
edge_t graph_find_edge_by_ids(graph_t g, vertexid_t id1, vertexid_t id2);
void graph_insert_edge(graph_t g, edge_t e);
void graph_print(graph_t g, int with_tuples);


#endif
