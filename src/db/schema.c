#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "schema.h"

char *base_types_str[] = {
	"CHAR", "VARCHAR", "BOOL", "INT", "FLOAT", "DOUBLE", "DATE", "TIME"
};

int base_types_len[] = {
	1, 256, 1, sizeof(long long int), sizeof(float),
	sizeof(double), 10, 8
};

void
schema_attribute_init(attribute_t attr, enum base_types bt, char *name)
{
	assert (attr != NULL);
	assert (name != NULL);

	memset(attr, 0, sizeof(struct attribute));
	attr->bt = bt;
	strcpy(attr->name, name);
	attr->next = NULL;
}

void
schema_attribute_print(attribute_t attr)
{
	assert (attr != NULL);

	printf("%s:%s", base_types_str[attr->bt], attr->name);
}

void
schema_attribute_insert(schema_t *s, attribute_t attr)
{
	assert (s != NULL);
	assert (attr != NULL);

	if (*s != NULL)
		attr->next = *s;

	*s = attr;
}

void
schema_attribute_remove(schema_t *s, attribute_t attr)
{
	schema_t prev, curr;

	assert (s != NULL);
	assert (attr != NULL);

	for (prev = NULL, curr = *s;
	     curr != NULL;
	     prev = curr, curr = curr->next)
		if (curr == attr) {
			if (prev == NULL) {
				*s = curr->next;
			} else {
				prev->next = curr->next;
			}
		}
}

int
schema_size(schema_t s)
{
	attribute_t attr;
	int acc = 0;

	for (attr = s; attr != NULL; attr = attr->next)
		acc += base_types_len[attr->bt];

	return acc;
}

void
schema_print(schema_t s)
{
	attribute_t attr;

	printf("{");
	for (attr = s; attr != NULL; attr = attr->next) {
		schema_attribute_print(attr);
		if (attr->next != NULL)
			printf(",");
	}
	printf("}");
}
