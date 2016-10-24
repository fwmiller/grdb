#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
	1,
	256,
	1,
	1,
	sizeof(int),
	sizeof(float),
	sizeof(double),
	10,
	8
};

void
schema_attribute_init(attribute_t attr, enum base_types bt, char *name)
{
	assert (attr != NULL);
	assert (name != NULL);

	memset(attr, 0, sizeof(struct attribute));
	attr->bt = bt;
	strcpy(attr->id.name, name);
	attr->next = NULL;
}

void
schema_attribute_print(attribute_t attr)
{
	assert (attr != NULL);

	printf("%s:%s", base_types_str[attr->bt], attr->id.name);
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

	if (*s != NULL)
		free(s);

	*s = malloc(sizeof(struct schema));
	memset(*s, 0, sizeof(struct schema));
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

	for (attr = s->attrlist; attr != NULL; attr = attr->next)
		if (strcasecmp(name, attr->id.name) == 0)
			return attr->bt;

	return BASE_TYPES_MAX;
}

void
schema_print(schema_t s)
{
	attribute_t attr;

	assert (s != NULL);

	printf("[");
	for (attr = s->attrlist; attr != NULL; attr = attr->next) {
		schema_attribute_print(attr);
		if (attr->next != NULL)
			printf(",");
	}
	printf("]");

	/* XXX Print enum list */
}
