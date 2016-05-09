#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "schema.h"

void
schema_attribute_alloc(attribute_t attr, enum base_types bt)
{
	assert (attr != NULL);

	memset(attr, 0, sizeof(struct attribute));
	attr->bt = bt;
	attr->next = NULL;
}

void
schema_attribute_print(attribute_t attr)
{
	int i;

	assert (attr != NULL);

	switch (attr->bt) {
	case CHARACTER:
		printf("CHAR(%d):[", attr->data.s.capacity);
		for (i = 0; i < attr->data.s.capacity; i++)
			printf("%c", attr->data.s.s[i]);
		printf("]");
		break;
	case VARCHAR:
		printf("VARCHAR(%d):[", attr->data.vs.capacity);
		for (i = 0; i < attr->data.vs.n; i++)
			printf("%c", attr->data.vs.s[i]);
		printf("]");
		break;
	case BOOLEAN:
		printf("BOOL:");
		if (attr->data.b == 0)
			printf("FALSE");
		else
			printf("TRUE");
		break;
	case INTEGER:
		printf("INT:%lld", attr->data.i);
		break;
	case FLOAT:
		printf("FLOAT:%4.2f", attr->data.f);
		break;
	case DOUBLE:
		printf("DOUBLE:%4.2f", attr->data.d);
		break;
	case DATE:
		printf("DATE:%02u/%02u/%04u",
		       attr->data.date.month,
		       attr->data.date.day,
		       attr->data.date.year);
		break;
	case TIME:
		printf("TIME:%02u:%02u:%02u",
		       attr->data.time.hours,
		       attr->data.time.minutes,
		       attr->data.time.seconds);
		printf("TIME");
		break;
	}
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

void
schema_print(schema_t s)
{
	attribute_t attr;

	for (attr = s; attr != NULL; attr = attr->next)
		schema_attribute_print(attr);
}
