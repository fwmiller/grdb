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
	int i;

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
		graph_print(current, 1); /* with tuples */
		printf("\n");
		return;
	}
	if (strlen(s2) <= 0) {
		printf("Missing name or id\n");
		return;
	}
	id1 = (vertexid_t) atoi(s1);

	/* Figure out if this is for an edge or vertex tuple */
	for (i = 0, st = EDGE; i < strlen(s2); i++)
		if (!isdigit(s2[i])) {
			st = VERTEX;
			break;
		}

	if (st == VERTEX) {
		vertex_t v;

		/*
		 * Set the value of a vertex tuple
		 */
		if (current->sv == NULL) {
			printf("Missing vertex schema\n");
			return;
		}

		v = graph_find_vertex_by_id(current, id1);
		if (v == NULL) {
			printf("Illegal vertex id\n");
			return;
		}
		if (tuple_set(v->tuple, s2, s3) < 0) {
			printf("Set tuple value failed\n");
			return;
		}


	} else if (st == EDGE) {
		/*
		 * Set the value of an edge tuple
		 */

	}
}
