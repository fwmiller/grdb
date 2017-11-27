#include <assert.h>
#if _DEBUG
#include <errno.h>
#endif
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "enum.h"


void
enum_init(enum_t *e)
{
	assert (e != NULL);

	if (*e != NULL)
		free(*e);

	*e = malloc(sizeof(struct grdb_enum));
	assert(*e != NULL);

	string_pool_init(&((*e)->pool));
	(*e)->next = NULL;
}

int
enum_file_open(char *grdbdir, int gidx, int cidx)
{
	char s[BUFSIZE];
	int fd;

	/* Create component enum file */
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/enum", grdbdir, gidx, cidx);
#if _DEBUG
	printf("enum_file_open: open enum file %s\n", s);
#endif
	fd = open(s, O_RDWR | O_CREAT, 0644);
#if _DEBUG
	if (fd < 0)
		printf("enum_file_open: open enum file failed (%s)\n",
			strerror(errno));
#endif
	return fd;
}

void
enum_print(enum_t e)
{
	printf("(");
	string_pool_print(e->pool);
	printf(")");
}

void
enum_insert(enum_t *e, char *s)
{
	assert (e != NULL);
	assert (*e != NULL);
	assert (s != NULL);

	string_pool_insert(&((*e)->pool), s);
}

char *enum_find_by_idx(enum_t e, int idx)
{
	return string_pool_find_by_idx(e->pool, idx);
}

int
enum_find_idx_by_name(enum_t e, char *s)
{
	return string_pool_find_idx_by_name(e->pool, s);
}

void
enum_set_name(enum_t e, char *name)
{
	assert (e != NULL);
	assert (name != NULL);

	strncpy(e->name, name, ENUM_NAME_LEN - 1);
}

char *
enum_get_name_ptr(enum_t e)
{
	assert (e != NULL);

	return e->name;
}

void
enum_list_init(enum_list_t *el)
{
	assert (el != NULL);
	*el = NULL;
}

int
enum_list_count(enum_list_t el)
{
	enum_t e;
	int n;

	assert (el != NULL);

	for (e = el, n = 0; e != NULL; e = e->next, n++);
	return n;
}

void
enum_list_print(enum_list_t el)
{
	enum_t e;

	for (e = el; e != NULL; e = e->next) {
		printf("%s ", e->name);
		enum_print(e);
		printf("\n");
	}
}

void
enum_list_insert(enum_list_t *el, enum_t e)
{
	assert (el != NULL);
	assert (e != NULL);

	if (*el != NULL)
		e->next = *el;
	*el = e;
}

enum_t
enum_list_find_by_name(enum_list_t el, char *name)
{
	enum_t e;

	for (e = el; e != NULL; e = e->next)
		if (strcasecmp(name, e->name) == 0)
			return e;
	return NULL;
}

int
enum_list_find_idx_by_name(enum_list_t el, char *name)
{
	enum_t e;
	int i;

	for (i = 0, e = el; e != NULL; i++, e = e->next)
		if (strcasecmp(name, e->name) == 0)
			return i;
	return (-1);
}

enum_t
enum_list_find_by_idx(enum_list_t el, int idx)
{
	enum_t e;
	int i;

	for (i = 0, e = el; e != NULL; i++, e = e->next)
		if (i == idx)
			return e;

	return NULL;
}
