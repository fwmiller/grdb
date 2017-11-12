#include <assert.h>
#include "import.h"

ivertex_t ivertex_select(igraph_t ig);
ivertex_t ivertex_neighbors(igraph_t ig, ivertex_t v);
ivertex_t ivertex_gcs(ivertex_t n, ivertex_t v, schema_t *sv, schema_t *se);
void component_grow(component_t c);
void igraph_project(igraph_t g, component_t c);
void igraph_edges();
void igraph_vertices();

/*
 * This routine will take as input a graph in general form, referred to
 * with the igraph_t type, and converts it to normal form, a set of
 * connected components with common schemas.
 *
 * The input graph is "consumed" as the routine proceeds, with the vertices
 * and edges being removed from the input graph data structure.  The routine
 * frees vertex and edge data structures as they are consumed.
 */

int
graph_import(igraph_t ig, graph_t g)
{
	ivertex_t n, u, v;
	schema_t sv = NULL, se = NULL;

	assert (ig != NULL);
	assert (g != NULL);

	/* XXX Assume g initialized to the set of empty components */

	/* Loop over the vertices until none remain */
	while (1) {
		/* If no vertices remain return success */
		if (ig->v == NULL)
			return 0;
		
		/* Select vertex u */
		u = ivertex_select(ig);
		if (u == NULL) {
			/* Error condition */
		}

		/* Compute the neighors of u */
		n = ivertex_neighbors(ig, u);

		/* Search neighbor v of u with the greatest common schema */
		v = ivertex_gcs(n, u, &sv, &se);

		/* If there is such a vertex then create new component */
		if (v != NULL) {
			component_t c;

			c = malloc(sizeof(struct component));
			assert (c != NULL);
			component_init(c);
			c->sv = sv;
			c->se = se;
			/* XXX Add one vertex */
			/* XXX Add second vertex */
			/* XXX Add edge */

			/* Try to grow the new component */
			component_grow(c);

			/* Project new component out of the input graph */
			igraph_project(ig, c);

			/* Insert new component in the normal form graph */
			if (g->c != NULL)
				c->next = g->c;
			g->c = c;

			continue;
		}
		/* Complete the edge components */
		igraph_edges();

		/* Complete the vertex components */
		igraph_vertices();

		return 0;
	}
}
