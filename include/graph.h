#ifndef __GRAPH_H
#define __GRAPH_H


#include <stdlib.h>
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

struct component {
	struct vertex *v;	/* Vertices */
	schema_t sv;		/* Vertex schema */
	struct edge *e;		/* Edges */
	schema_t se;		/* Edge schema */
	enum_list_t el;		/* List of enums */

	/* Links for component data structure in memory */
	struct component *prev;
	struct component *next;

	/* Secondary storage file handles */
	int vfd;		/* Vertex file */
	int efd;		/* Edge file */
};

struct graph {
	struct component *c;	/* List of components */

	/* Links for graph data structure in memory */
	struct graph *prev;
	struct graph *next;
};

typedef struct vertex *vertex_t;
typedef struct edge *edge_t;
typedef struct component *component_t;
typedef struct graph *graph_t;


void vertex_init(vertex_t v);
int vertex_file_init(int gidx, int cidx);
void vertex_set_id(vertex_t v, vertexid_t id);
void vertex_print(vertex_t v);
ssize_t vertex_read(vertex_t v, schema_t schema, int fd);
ssize_t vertex_write(vertex_t v, int fd);


void edge_init(edge_t e);
int edge_file_init(int gidx, int cidx);
void edge_set_vertices(edge_t e, vertexid_t id1, vertexid_t id2);
void edge_print(edge_t e);
ssize_t edge_read(edge_t e, schema_t schema, int fd);
ssize_t edge_write(edge_t e, int fd);

void component_init(component_t c);
void component_file_init(int gidx, int cidx);
void component_insert_vertex(component_t c, vertex_t v);
vertex_t component_find_vertex_by_id(component_t c, vertex_t v);
edge_t component_find_edge_by_ids( component_t c, edge_t e);
void component_insert_edge(component_t c, edge_t e);
void component_print(component_t c, int with_tuples);

/*
 * Project a component from the input component c.  The resulting
 * component has only the attributes listed in the attrlist.
 */
component_t
component_project(component_t c, attribute_t attrlist);

/*
 * Select a component from the input compnent c
 *
 * The resulting component must be connected or the select operation
 * fails.
 */
component_t
component_select();

/*
 * Join two components, c1 and c2, together based on their vertex ids.
 * The resulting component should contain vertices that are the union
 * of those in c1 and c2.  It should also contain the edges from both
 * c1 and c2.  The schema for vertices and the edges should be the
 * the unions of the vertex schema for c1 and c2 and the edges schema
 * for c1 and c2, respectively.
 * The resulting component must be connected or the join operation
 * fails.
 */
component_t
component_join(component_t c1, component_t c2);

/* 
 * Execute Dijkstra's algorithm on the specified component.  Find the
 * shortest path from v1 to v2 if it exists in the component c.
 *
 * For CSCI 3287:  Assume that the path will have at least one integer
 * attribute that is the same along the edges connecting the two vertices
 * if a path exists.  You should be able to find an integer attribute
 * in the edge schema associated with the component.
 *
 * The routine returns the value 0 if the algorithm executes successfully
 * and the value (-1) otherwise.  If a correct result is obtained an
 * array is returned through the other parameters.  The reference parameter
 * n is used to return the number of vertices on the path, including the
 * two specified endpoints.  The reference parameter total_weight is used 
 * to return the total_weight of summed along the edges of the path.  The
 * path reference parameter returns an array of vertex ids that contain
 * the shortest path, including the two specified endpoints.
 */
int
component_sssp(
	component_t c,
	vertexid_t v1,
	vertexid_t v2,
	int *n,
	int *total_weight,
	vertexid_t **path);


void graph_init(graph_t g);
void graph_file_init(int gidx);
void graph_insert_component(graph_t g, component_t c);


#endif
