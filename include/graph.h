#ifndef __GRAPH_H
#define __GRAPH_H


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


typedef struct vertex *vertex_t;


void vertex_alloc(vertex_t v);
void vertex_set_id(vertex_t v, vertexid_t id);
void vertex_edge_insert(vertex_t v, vertexid_t id);
void vertex_edge_remove(vertex_t v, vertexid_t id);
void vertex_print(vertex_t v);

void graph_insert_vertex(vertex_t *g, vertex_t v);
vertex_t graph_find_vertex_by_id(vertex_t g, vertexid_t id);
void graph_insert_edge(vertex_t g, vertexid_t v1, vertexid_t v2);
void graph_print(vertex_t g);

#endif
