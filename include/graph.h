#ifndef __GRAPH_H
#define __GRAPH_H


#include "schema.h"
#include "vertexid.h"


struct vertex {
	/* Every vertex has a unique vertex id */
	vertexid_t id;

	/* The vertex adjacency set */
	struct vertexid_set adj;

	/* Links for vertex set data structure in memory */
	struct vertex *prev;
	struct vertex *next;

	/*** User data here ***/
};

struct graph {
	struct vertex *v;	/* Vertices */
	schema_t sv;		/* Vertex schema */
	schema_t se;		/* Edge schema */
};

typedef struct vertex *vertex_t;
typedef struct graph *graph_t;

void vertex_alloc(vertex_t v);
void vertex_set_id(vertex_t v, vertexid_t id);
void vertex_edge_insert(vertex_t v, vertexid_t id);
void vertex_edge_remove(vertex_t v, vertexid_t id);
void vertex_print(vertex_t v);

void graph_insert_vertex(graph_t g, vertex_t v);
vertex_t graph_find_vertex_by_id(graph_t g, vertexid_t id);
void graph_insert_edge(graph_t g, vertexid_t v1, vertexid_t v2);
void graph_print(graph_t g);

#endif
