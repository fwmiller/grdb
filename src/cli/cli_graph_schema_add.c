#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "cli.h"

void cli_graph_update_tuples(schema_type_t st, int old_schema_size);

static void
cli_graph_schema_add_enum(
	schema_t schema,
	int old_schema_size,
	schema_type_t st,
	enum_t e,
	char *name)
{
	attribute_t attr;

	assert (schema != NULL);

#if _DEBUG
	printf("add attribute type enum %s name %s to schema\n",
	       e->name, name);
#endif
	attr = (attribute_t)
		malloc(sizeof(struct attribute));
	assert(attr != NULL);
	schema_attribute_init(attr, name, ENUM, e);
	strncpy(attr->name, name, ATTR_NAME_MAXLEN - 1);
	schema_attribute_insert(schema, attr);
#if _DEBUG
	printf("update tuples with enum values\n");
#endif
	cli_graph_update_tuples(st, old_schema_size);
}

static void
cli_graph_schema_add_base(
	schema_t schema,
	int old_schema_size,
	schema_type_t st,
	char *type,
	char *name)
{
	int i;

	assert (schema != NULL);

	for (i = 0; i < BASE_TYPES_MAX; i++) {
		if (strcasecmp(type, base_types_str[i]) == 0) {
#if _DEBUG
			printf("add %s to schema\n", type);
#endif
			attribute_t attr;
	
			attr = (attribute_t)
				malloc(sizeof(struct attribute));
			assert(attr != NULL);
			schema_attribute_init(attr, name, i, NULL);
			schema_attribute_insert(schema, attr);
#if _DEBUG
			printf("update tuples\n");
#endif
			cli_graph_update_tuples(st, old_schema_size);

			break;
		}
	}
}

void
cli_graph_schema_add(schema_type_t st, char *cmdline, int *pos)
{
	char s[BUFSIZE];
	schema_t schema = NULL;
	char type[BUFSIZE];
	char name[BUFSIZE];
	int old_schema_size = 0;
	enum_list_t el = NULL;
	enum_t e;
	int fd, efd;

	/* Load enums */
	efd = enum_file_open(grdbdir, gno, cno);
	if (efd < 0)
		return;

	enum_list_init(&el);
	enum_list_read(&el, efd);

	/* Load the appropriate schema */
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/%s",
		grdbdir, gno, cno, (st == VERTEX ? "sv" : "se"));
	fd = open(s, O_RDWR | O_CREAT, 0644);
	if (fd < 0)
		return;

	schema = schema_read(fd, el);
	if (schema == NULL)
		old_schema_size = 0;
	else
		old_schema_size = schema_size(schema);

	/* Attribute type */
        memset(type, 0, BUFSIZE);
        nextarg(cmdline, pos, " ", type);

	/* Attribute name */
        memset(name, 0, BUFSIZE);
        nextarg(cmdline, pos, " ", name);

	/* Search enums for type name */
	e = enum_list_find_by_name(el, type);
	if (e != NULL) {
#if _DEBUG
		printf("add enum %s\n", name);
#endif
		if (schema == NULL)
			schema_init(&schema);
		cli_graph_schema_add_enum(
			schema, old_schema_size, st, e, name);

		enum_list_write(el, efd);
		close(efd);

		schema_write(schema, fd);
		close(fd);
		return;
	}
	close(efd);

#if _DEBUG
	printf("add attribute %s with base type %s\n", name, type);
#endif
	if (schema == NULL)
		schema_init(&schema);
	cli_graph_schema_add_base(schema, old_schema_size, st, type, name);
	schema_write(schema, fd);
	close(fd);
}
