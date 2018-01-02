#include <ctype.h>
#if _DEBUG
#include <errno.h>
#endif
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "cli.h"

static void
cli_graph_vertex_write(vertex_t v)
{
	int fd;
	char s[BUFSIZE];

	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/v", grdbdir, gno, cno);
#if _DEBUG
	printf("cli_graph_vertex_write: open vertex file %s\n", s);
#endif
	fd = open(s, O_RDWR);
	if (fd < 0) {
#if _DEBUG
		printf("Open vertex file failed (%s)\n",
			strerror(errno));
#endif
		return;
	}
	vertex_write(v, fd);
	close(fd);
}

static void
cli_graph_edge_write(edge_t e)
{
	int fd;
	char s[BUFSIZE];

	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/e", grdbdir, gno, cno);
#if _DEBUG
	printf("cli_graph_edge_write: open edge file %s\n", s);
#endif
	fd = open(s, O_RDWR);
	if (fd < 0) {
#if _DEBUG
		printf("Open edge file failed (%s)\n",
			strerror(errno));
#endif
		return;
	}
	edge_write(e, fd);
	close(fd);
}

void
cli_graph_tuple(char *cmdline, int *pos)
{
	char s1[BUFSIZE], s2[BUFSIZE], s3[BUFSIZE], s4[BUFSIZE];
	vertexid_t id1;
	schema_type_t st;
	int i, n, len;
	char ch;

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
		FILE *out;
		char s[BUFSIZE];

		memset(s, 0, BUFSIZE);
		sprintf(s, "/tmp/grdbGraphs");
		out = fopen(s, "w");
		if (out == NULL) {
			printf("Fopen %s failed\n", s);
			return;
		}
		memset(s, 0, BUFSIZE);
		sprintf(s, "%d", gno);
		cli_components_print(out, s, 1); /* with tuples */

		fclose(out);
		out = NULL;

		memset(s, 0, BUFSIZE);
		sprintf(s, "/tmp/grdbGraphs");
		out = fopen(s, "r");
		if (out == NULL) {
			printf("Fopen %s failed\n", s);
			return;
		}
		for (;;) {
			len = fread(&ch, 1, 1, out);
			if (len <= 0)
				break;
			printf("%c", ch);
		}
		fclose(out);

		/* Remove the file */
		//unlink(s);
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
		struct component c;
		struct vertex v;
		vertex_t v1;
		base_types_t bt;
		int fd;
		char s[BUFSIZE];

		/* Setup a component for searching */
		component_init(&c);

		/* Load enums */
		fd = enum_file_open(grdbdir, gno, cno);
		if (fd < 0) {
			printf("Open enum file failed\n");
			return;
		}
		enum_list_init(&(c.el));
		enum_list_read(&(c.el), fd);
		close(fd);

		/* Load the vertex schema */
		memset(s, 0, BUFSIZE);
		sprintf(s, "%s/%d/%d/sv", grdbdir, gno, cno);
#if _DEBUG
		printf("cli_graph_tuple: read vertex schema file %s\n", s);
#endif
		fd = open(s, O_RDWR | O_CREAT, 0644);
		if (fd < 0) {
			printf("Open vertex schema file failed\n");
			return;
		}
		c.sv = schema_read(fd, c.el);
		close(fd);

		/* Set the value of a vertex tuple */
		vertex_init(&v);
		v.id = id1;

		/* Open the vertex file */
		memset(s, 0, BUFSIZE);
		sprintf(s, "%s/%d/%d/v", grdbdir, gno, cno);
#if _DEBUG
		printf("cli_graph_tuple: open vertex file %s\n", s);
#endif
		c.vfd = open(s, O_RDWR | O_CREAT, 0644);
		if (c.vfd < 0) {
			printf("Open vertex file failed\n");
			return;
		}
		v1 = component_find_vertex_by_id(&c, &v);
		if (v1 == NULL) {
			printf("Find vertex id %llu failed\n", id1);
			return;
		}
		close(c.vfd);

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
				attr->e->name, s3, c.el);

			cli_graph_vertex_write(v1);
			return;
		}
		if (tuple_set(v1->tuple, s2, s3) < 0) {
			printf("Set vertex tuple value failed\n");
			return;
		}
		cli_graph_vertex_write(v1);


	} else if (st == EDGE) {
		struct component c;
		struct edge e;
		edge_t e1;
		vertexid_t id2;
		base_types_t bt;
		int fd;
		char s[BUFSIZE];

		/* Setup a component for searching */
		component_init(&c);

		/* Load enums */
		fd = enum_file_open(grdbdir, gno, cno);
		if (fd < 0) {
			printf("Open enum file failed\n");
			return;
		}
		enum_list_init(&(c.el));
		enum_list_read(&(c.el), fd);
		close(fd);

		/* Load the edge schema */
		memset(s, 0, BUFSIZE);
		sprintf(s, "%s/%d/%d/se", grdbdir, gno, cno);
#if _DEBUG
		printf("cli_graph_tuple: read edge schema file %s\n", s);
#endif
		fd = open(s, O_RDWR | O_CREAT, 0644);
		if (fd < 0) {
			printf("Open edge schema file failed\n");
			return;
		}
		c.se = schema_read(fd, c.el);
		close(fd);

		/* s2 is a vertex id for an edge */
		id2 = (vertexid_t) atoi(s2);

		/* Open the edge file */
		memset(s, 0, BUFSIZE);
		sprintf(s, "%s/%d/%d/e", grdbdir, gno, cno);
#if _DEBUG
		printf("cli_graph_tuple: open edge file %s\n", s);
#endif
		c.efd = open(s, O_RDWR | O_CREAT, 0644);
		if (c.efd < 0) {
			printf("Find edge ids (%llu,%llu) failed\n",
				id1, id2);
			return;
		}
		edge_init(&e);
		edge_set_vertices(&e, id1, id2);
		e1 = component_find_edge_by_ids(&c, &e);
		if (e1 == NULL) {
			printf("Illegal edge id(s)\n");
			return;
		}
		close(c.efd);

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
				attr->e->name, s4, c.el);

			cli_graph_edge_write(e1);
			return;
		}
		if (tuple_set(e1->tuple, s3, s4) < 0) {
			printf("Set edge tuple value failed\n");
			return;
		}
		cli_graph_edge_write(e1);
	}
}
