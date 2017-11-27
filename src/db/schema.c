#include <assert.h>
#if _DEBUG
#include <errno.h>
#endif
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "schema.h"

char *base_types_str[] = {
	"CHAR",
	"VARCHAR",
	"BOOL",
	"ENUM",
	"INT",
	"FLOAT",
	"DOUBLE",
	"DATE",
	"TIME"
};

int base_types_len[] = {
	1,			/* Single char */
	256,			/* Max string length */
	1,			/* Unsigned char 0=FALSE 1=TRUE */
	2,			/* Two unsigned chars 1) enum index */
				/*   2) item index */
	8,			/* For 64-bit */
	sizeof(float),		/* Should be an IEEE standard length */
	sizeof(double),		/* Should be an IEEE standard length */
	10,			/* "MM-DD-YYYY" string no terminator */
	8			/* "HH:MM:SE" string no terminator */
};

void
schema_attribute_init(
	attribute_t attr,
	void *name,
	enum base_types bt,
	enum_t e)
{
	assert (attr != NULL);
	assert (name != NULL);

	memset(attr, 0, sizeof(struct attribute));
	strncpy(attr->name, name, ATTR_NAME_MAXLEN - 1);
	attr->bt = bt;
	attr->e = e;
	attr->next = NULL;
}

void
schema_attribute_print(attribute_t attr, enum_list_t el)
{
	assert (attr != NULL);

	printf("%s:%s",
	       (attr->bt == ENUM ?
		attr->e->name :
		base_types_str[attr->bt]),
	       attr->name);
}

void
schema_attribute_insert(schema_t s, attribute_t attr)
{
	attribute_t t;

	assert (s != NULL);
	assert (attr != NULL);

	if (s->attrlist == NULL) {
		s->attrlist = attr;
		return;
	}
	for (t = s->attrlist; t->next != NULL; t = t->next);
	t->next = attr;
}

attribute_t
schema_attribute_last(schema_t s)
{
	attribute_t attr;

	if (s == NULL || s->attrlist == NULL)
		return NULL;

	for (attr = s->attrlist; attr->next != NULL; attr = attr->next);
	return attr;
}

void
schema_attribute_remove(schema_t s, attribute_t attr)
{
	attribute_t prev, curr;

	assert (s != NULL);
	assert (attr != NULL);

	for (prev = NULL, curr = s->attrlist;
	     curr != NULL;
	     prev = curr, curr = curr->next)
		if (curr == attr) {
			if (prev == NULL) {
				s->attrlist = curr->next;
			} else {
				prev->next = curr->next;
			}
		}
}

void
schema_init(schema_t *s)
{
	assert (s != NULL);
#if 0
	/* XXX Not having something like this could be a memory leak */
	if (*s != NULL)
		free(s);
#endif
	*s = malloc(sizeof(struct schema));
	memset(*s, 0, sizeof(struct schema));
}

int
schema_file_init(int gidx, int cidx, char *name)
{
	char s[BUFSIZE];
	int fd;

	/* Create component schema file */
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/%s", GRDBDIR, gidx, cidx, name);
#if _DEBUG
	printf("schema_file_init: open schema file %s\n", s);
#endif
	fd = open(s, O_RDWR | O_CREAT, 0644);
#if _DEBUG
	if (fd < 0)
		printf("schema_file_init: open schema file failed (%s)\n",
			strerror(errno));
#endif
	return fd;
}

int
schema_count(schema_t s)
{
	attribute_t attr;
	int cnt;

	assert(s != NULL);

	for (attr = s->attrlist, cnt = 0;
	     attr != NULL;
	     attr = attr->next, cnt++);

	return cnt;
}

int
schema_size(schema_t s)
{
	attribute_t attr;
	int acc = 0;

	assert(s != NULL);

	for (attr = s->attrlist; attr != NULL; attr = attr->next)
		acc += base_types_len[attr->bt];
	return acc;
}

base_types_t
schema_find_type_by_name(schema_t s, char *name)
{
	attribute_t attr;

	assert (s != NULL);

#if _DEBUG
	printf("find schema [%s]\n", name);
#endif
	for (attr = s->attrlist; attr != NULL; attr = attr->next)
		if (strcasecmp(name, attr->name) == 0)
			return attr->bt;

	return BASE_TYPES_MAX;
}

attribute_t
schema_find_attr_by_name(schema_t s, char *name)
{
	attribute_t attr;

	assert (s != NULL);

#if _DEBUG
	printf("find attribute [%s]\n", name);
#endif
	for (attr = s->attrlist; attr != NULL; attr = attr->next)
		if (strcasecmp(name, attr->name) == 0)
			return attr;

	return NULL;
}

void
schema_print(schema_t s, enum_list_t el)
{
	attribute_t attr;

	assert (s != NULL);

	printf("[");
	for (attr = s->attrlist; attr != NULL; attr = attr->next) {
		schema_attribute_print(attr, el);
		if (attr->next != NULL)
			printf(",");
	}
	printf("]");

	/* XXX Print enum list */
}
