#include <assert.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include "cli.h"
#include "config.h"
#include "graph.h"


static int
component_union_enums(
	int cidx1, int cidx2, char *grdbdir, int gno, int cidx)
{
	enum_list_t el1 = NULL, el2 = NULL;
	struct stat st = {0};
	char s[BUFSIZE];
	int fd;

	/* Check whether a new component directory needs to be created */
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d", grdbdir, gno, cidx);
	if (stat(s, &st) < 0)
		mkdir(s, 0777);

	/* Read in enums for component 1 */
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/enum", grdbdir, gno, cidx1);
	fd = open(s, O_RDONLY);
	if (fd < 0) {
#if _DEBUG
		printf("component_union: open %s failed\n", s);
#endif
		return (-1);
	}
	enum_list_read(&el1, fd);
	close(fd);
	
	/* Read in enums for component 2 */
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/enum", grdbdir, gno, cidx2);
	fd = open(s, O_RDONLY);
	if (fd < 0) {
#if _DEBUG
		printf("component_union: open %s failed\n", s);
#endif
		return (-1);
	}
	enum_list_read(&el2, fd);
	close(fd);

	/* Union the enums */
	enum_list_union(el1, el2, grdbdir, gno, cidx);
	return 0;
}

static int
component_union_schemas(
	int cidx1,
	int cidx2,
	char *grdbdir,
	int gno,
	int cidx)
{
	enum_list_t el1 = NULL, el2 = NULL;
	schema_t s1 = NULL, s2 = NULL;
	char s[BUFSIZE];
	int fd, result;

	/* Read in enums for component 1 */
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/enum", grdbdir, gno, cidx1);
	fd = open(s, O_RDONLY);
	if (fd >= 0) {
		enum_list_read(&el1, fd);
		close(fd);
	}
	/* Read in vertex schema for component 1 */
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/sv", grdbdir, gno, cidx1);
	fd = open(s, O_RDONLY);
	if (fd >= 0) {
		s1 = schema_read(fd, el1);
		close(fd);
	}
	/* Read in enums for component 2 */
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/enum", grdbdir, gno, cidx2);
	fd = open(s, O_RDONLY);
	if (fd >= 0) {
		enum_list_read(&el2, fd);
		close(fd);
	}
	/* Read in vertex schema for component 2 */
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/sv", grdbdir, gno, cidx2);
	fd = open(s, O_RDONLY);
	if (fd >= 0) {
		s2 = schema_read(fd, el2);
		close(fd);
	}
	/* Union the vertex schemas */
	result = schema_union(VERTEX, s1, s2, grdbdir, gno, cidx);
	if (result < 0) {
#if _DEBUG
		printf("component_union: vertex schema union failed\n");
#endif
		return (-1);
	}
	/* Read in edge schema for component 1 */
	s1 = NULL;
	s2 = NULL;
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/se", grdbdir, gno, cidx1);
	fd = open(s, O_RDONLY);
	if (fd >= 0) {
		s1 = schema_read(fd, el1);
		close(fd);
	}
	/* Read in edge schema for component 2 */
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/se", grdbdir, gno, cidx2);
	fd = open(s, O_RDONLY);
	if (fd >= 0) {
		s2 = schema_read(fd, el2);
		close(fd);
	}
	/* Union the vertex schemas */
	result = schema_union(EDGE, s1, s2, grdbdir, gno, cidx);
	if (result < 0) {
#if _DEBUG
		printf("component_union: edge schema union failed\n");
#endif
		return (-1);
	}
	return 0;
}

static int
component_union_structures(
	int cidx1,
	int cidx2,
	char *grdbdir,
	int gno,
	int cidx)
{
	/* Open new vertex file for unioned component */

	/* Open first input component vertex file */

	/* Copy all the vertices to the new vertex file */

	/* Open second input component vertex file */

	/* Concatenate all the vertices to the end of new vertex file */


	/* XXX Need to do duplicate elimination on the new vertex file */


	return (-1);
}

/*
 * This function does two things.  1) It builds the enums and vertex and
 * edge schemas from the two input components and 2) It builds a new
 * component structure, including tuples, based on the structures of the
 * two input components.
 */
int
component_union(int cidx1, int cidx2, char *grdbdir, int gno)
{
	struct stat st = {0};
	char s[BUFSIZE];
	int cidx, result;

	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d", grdbdir, gno, cidx1);
	if (stat(s, &st) < 0) {
#if _DEBUG
		printf("component_union: stat %s failed\n", s);
#endif
		return (-1);
	}
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d", grdbdir, gno, cidx2);
	if (stat(s, &st) < 0) {
#if _DEBUG
		printf("component_union: stat %s failed\n", s);
#endif
		return (-1);
	}
	/* Get the next component number for the current graph */
	cidx = graph_next_cno(grdbdir, gno);

	/* Union enums */
	result = component_union_enums(cidx1, cidx2, grdbdir, gno, cidx);
	if (result < 0) {
#if _DEBUG
		printf("component_union: union enums failed\n");
#endif
		return (-1);
	}

	/* Union schemas */
	result = component_union_schemas(cidx1, cidx2, grdbdir, gno, cidx);
	if (result < 0) {
#if _DEBUG
		printf("component_union: union schemas failed\n");
#endif
		return (-1);
	}

	/* Union structures */

	return 0;

#if 0
	fd1 = open(s, O_RDONLY);

	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d", grdbdir, gno, cidx2);
	fd2 = open(s, O_RDONLY);
	if (fd2 < 0) {
		return (-1);
	}
	/* Get the next component number for the current graph */
	cidxnext = graph_next_cno(grdbdir, gno);

	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d", grdbdir, gno, cidxnext);
	fdnew = open(s, O_WRONLY | O_CREAT, 0644);
	if (fdnew < 0) {
		return (-1);
	}

	close(fdnew);
	close(fd2);
	close(fd1);

	return 0;

	/*
	 * XXX Find the highest numbered component in the current graph
	 * directory
	 */

	/* Create a new component directory */
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/123",grdbdir, gno);
	if (stat(s, &st) < 0)
		mkdir(s, 0777);

	/* Join enums */
	temp.el = enum_list_union(c1->el, c2->el);
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/123/enum",grdbdir, gno);
	fd = open(s, O_WRONLY | O_CREAT, 0644);
	enum_list_write(temp.el, fd);
	close(fd);

	/* Join vertex schema */
	temp.sv = schema_union(c1->sv, c2->sv);
	if (temp.sv != NULL) {
		memset(s, 0, BUFSIZE);
		sprintf(s, "%s/%d/123/sv",grdbdir, gno);
		fd = open(s, O_WRONLY | O_CREAT, 0644);
		schema_write(temp.sv, fd);
		close(fd);
	}
	/* Join edge schema */
	temp.se = schema_union(c1->se, c2->se);
	if (temp.se != NULL) {
		memset(s, 0, BUFSIZE);
		sprintf(s, "%s/%d/123/se",grdbdir, gno);
		fd = open(s, O_WRONLY | O_CREAT, 0644);
		schema_write(temp.se, fd);
		close(fd);
	}
	/* Join component structures including tuples */
	if (component_union_structure(c1, c2) == NULL)
		return (-1);

	return 0;
#endif
}
