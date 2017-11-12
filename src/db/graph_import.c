#include "import.h"

ivertex_t ivertex_select(igraph_t ig);
ivertex_t ivertex_neighbors(igraph_t ig, ivertex_t v);
ivertex_t ivertex_gcs(ivertex_t n, ivertex_t v);

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
		v = ivertex_gcs(n, u);

		/* If there is such a vertex then create new component */
		if (v == NULL) {
			/* Try to grow the new component */

			/* Project new component out of the input graph */

			/* Insert new component in the normal form graph */

			continue;
		}
		/* Complete the edge components */

		/* Complete the vertex components */

		/* Return success */

	}
}
