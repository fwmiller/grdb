#include "graph.h"

/*
 * This function combines two component structures.
 */
static component_t
component_join_structure(component_t c1, component_t c2)
{
	return NULL;
}


/*
 * This function does two things.  1) It builds the enums and vertex and
 * edge schemas from the two input components and 2) It builds a new
 * component structure, including tuples, based on the structures of the
 * two input components.
 */
component_t
component_join(component_t c1, component_t c2)
{
	component_t c = NULL;

	/* Join enums */

	/*
	 * Join vertex schema.  The first schema is concatentated with
	 * the second schema.  This order is assumed in the component
	 * structure join
	 */

	/*
	 * Join edge schema.  The first schema is concatentated with
	 * the second schema.  This order is assumed in the component
	 * structure join
	 */

	/* Join component structures including tuples */
	c = component_join_structure(c1, c2);

	/* Assign enums and schemas to new component */

	return c;
}
