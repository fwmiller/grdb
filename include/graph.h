#ifndef __GRAPH_H
#define __GRAPH_H


#include <dirent.h>
#include <stdio.h>
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
void vertex_print(FILE *out, vertex_t v);
void vertex_copy_by_field(char *buf1, schema_t s1, char *buf2, schema_t s2);
ssize_t vertex_read(vertex_t v, schema_t schema, int fd);
ssize_t vertex_write(vertex_t v, int fd);

void vertex_write_from(
        vertexid_t id,
        char *buf,
        schema_t schema,
        int fdnew,
        schema_t schemanew);

void edge_init(edge_t e);
int edge_file_init(int gidx, int cidx);
void edge_set_vertices(edge_t e, vertexid_t id1, vertexid_t id2);
void edge_print(FILE *out, edge_t e);
ssize_t edge_read(edge_t e, schema_t schema, int fd);
ssize_t edge_write(edge_t e, int fd);

void component_init(component_t c);
void component_file_init(int gidx, int cidx);
void component_insert_vertex(component_t c, vertex_t v);
vertex_t component_find_vertex_by_id(component_t c, vertex_t v);
edge_t component_find_edge_by_ids( component_t c, edge_t e);
void component_insert_edge(component_t c, edge_t e);
void component_print(FILE *out, component_t c, int with_tuples);

/*
 * Determine if two vertices are connected withing a specified component.
 * The strong routine determines if there is a directed path between the
 * the two vertices.  The weak routine determines if there is a path
 * that treats the edges as undirected between the vertices.
 */
int component_connected_strong(
	char *grdbdir, int gidx, int cidx, vertexid_t id1, vertexid_t id2);
int component_connected_weak(
	char *grdbdir, int gidx, int cidx, vertexid_t id1, vertexid_t id2);

/*
 * Project a component from the input component c.  The resulting
 * component has only the attributes listed in the attrlist.
 */
component_t component_project(component_t c, attribute_t attrlist);

/*
 * Select a component from the input compnent c
 *
 * The resulting component must be connected or the select operation
 * fails.
 */
component_t component_select();

/*
 * Union two components, c1 and c2, together based on their vertex ids.
 * The resulting component should contain vertices that are the union
 * of those in c1 and c2.  It should also contain the edges from both
 * c1 and c2.  The schema for vertices and the edges should be the
 * the unions of the vertex schema for c1 and c2 and the edges schema
 * for c1 and c2, respectively.
 *
 * The resulting component must be connected or the union operation
 * fails.
 *
 * The routine returns the value 0 if the union was successful and
 * (-1) if it fails.  If successful, the graph number and component
 * number of the new component are returned through the gidx and
 * cidx reference parameters, respectively.
 */
int component_union(int cidx1, int cidx2, char *grdbdir, int gno);


/* 
 * Execute Dijkstra's algorithm on the specified component.  Find the
 * shortest path from v1 to v2 if it exists in the component c.
 *
 * For CSCI 5817:
 *   Assume that the weight_file contains the name of the edge attribute
 *   to be used as the integer weight for Dijkstra.
 *
 *   Your implementation should include _DEBUG statements that print
 *   the total weight for the path and the edges on the path in order.
 *
 * The routine returns the value 0 if the algorithm executes successfully
 * and the value (-1) otherwise.
 */
int
component_sssp(
	char *grdbdir,
	int gidx,
	int cidx,
	vertexid_t id1,
	vertexid_t id2,
	char *weight_field);


void graph_init(graph_t g);
void graph_file_init(int gidx);
void graph_insert_component(graph_t g, component_t c);
int graph_next_cno(char *grdbdir, int gno);


#endif
