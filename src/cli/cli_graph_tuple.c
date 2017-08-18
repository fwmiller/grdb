#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"

void
cli_graph_tuple(char *cmdline, int *pos)
{
	char s1[BUFSIZE], s2[BUFSIZE], s3[BUFSIZE], s4[BUFSIZE];
	vertexid_t id1;
	schema_type_t st;
	int i, n;

	memset(s1, 0, BUFSIZE);
	nextarg(cmdline, pos, " ", s1);
	memset(s2, 0, BUFSIZE);
	nextarg(cmdline, pos, " ", s2);
	memset(s3, 0, BUFSIZE);
	nextarg(cmdline, pos, " ", s3);
	memset(s4, 0, BUFSIZE);
	nextarg(cmdline, pos, " ", s4);
#if _DEBUG
	printf("s1=[%s] s2=[%s] s3=[%s], s4=[%s]\n", s1, s2, s3, s4);
#endif
	if (strlen(s1) <= 0) {
		char s[BUFSIZE];

		memset(s, 0, BUFSIZE);
		sprintf(s, "%d", gno);
		cli_components_print(s);
		return;
	}
	if (strlen(s2) <= 0) {
		printf("Missing name or id\n");
		return;
	}
	/* s1 is always a vertex id */
	id1 = (vertexid_t) atoi(s1);

	/* Figure out if this is for an edge or vertex tuple */
	n = strlen(s2);
	for (i = 0, st = EDGE; i < n; i++)
		if (!isdigit(s2[i])) {
			st = VERTEX;
			break;
		}

	if (st == VERTEX) {
		struct vertex v;
		vertex_t v1;
		base_types_t bt;

		/*
		 * Set the value of a vertex tuple
		 */
		if (current_component == NULL ||
		    current_component->sv == NULL) {
			printf("Missing vertex schema\n");
			return;
		}

		vertex_init(&v);
		v.id = id1;
		v1 = component_find_vertex_by_id(current_component, &v);
		if (v1 == NULL) {
			printf("Illegal vertex id\n");
			return;
		}
		/* s2 is an attribute name from the vertex schema */

		/* Check for a VARCHAR */
		bt = schema_find_type_by_name(v1->tuple->s, s2);
		if (bt == VARCHAR) {
			char *first, *second;

			first = strchr(cmdline, '"');
			if (first == NULL) {
				printf("Missing first quote");
				return;
			}
			second = strchr(first + 1, '"');
			if (second == NULL) {
				printf("Missing last quote");
				return;
			}
			memset(s3, 0, BUFSIZE);
			strncpy(s3, first + 1, second - first - 1);
#if _DEBUG
			printf("s3=[%s]\n", s3);
#endif
		} else if (bt == ENUM) {
			attribute_t attr;

			attr = schema_find_attr_by_name(v1->tuple->s, s2);
			if (attr == NULL) {
				printf("Attribute %s not found\n", s2);
				return;
			}
#if _DEBUG
			printf("set attribute %s with type %s to %s\n",
			       s2, attr->e->name, s3);
#endif
			tuple_set_enum(v1->tuple, s2,
				attr->e->name, s3, current_component->el);
			return;
		}
		if (tuple_set(v1->tuple, s2, s3) < 0) {
			printf("Set vertex tuple value failed\n");
			return;
		}


	} else if (st == EDGE) {
		struct edge e;
		edge_t e1;
		vertexid_t id2;
		base_types_t bt;

		/*
		 * Set the value of an edge tuple
		 */
		if (current_component == NULL ||
		    current_component->se == NULL) {
			printf("Missing edge schema\n");
			return;
		}
		/* s2 is a vertex id for an edge */
		id2 = (vertexid_t) atoi(s2);

		edge_init(&e);
		edge_set_vertices(&e, id1, id2);
		e1 = component_find_edge_by_ids(current_component, &e);
		if (e1 == NULL) {
			printf("Illegal vertex id(s)\n");
			return;
		}
		/* Check for a VARCHAR */
		bt = schema_find_type_by_name(e1->tuple->s, s3);
		if (bt == VARCHAR) {
			char *first, *second;

			first = strchr(cmdline, '"');
			if (first == NULL) {
				printf("Missing first quote");
				return;
			}
			second = strchr(first + 1, '"');
			if (second == NULL) {
				printf("Missing last quote");
				return;
			}
			memset(s4, 0, BUFSIZE);
			strncpy(s4, first + 1, second - first - 1);
#if _DEBUG
			printf("s4=[%s]\n", s4);
#endif
		} else if (bt == ENUM) {
			attribute_t attr;

			attr = schema_find_attr_by_name(e1->tuple->s, s3);
			if (attr == NULL) {
				printf("Attribute %s not found\n", s3);
				return;
			}
#if _DEBUG
			printf("set attribute %s with type %s to %s\n",
			       s3, attr->e->name, s4);
#endif
			tuple_set_enum(e1->tuple, s3,
				attr->e->name, s4, current_component->el);
			return;
		}
		if (tuple_set(e1->tuple, s3, s4) < 0) {
			printf("Set edge tuple value failed\n");
			return;
		}
	}
}
