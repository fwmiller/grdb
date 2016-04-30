#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "schema.h"

void
schema_element_alloc(element_t elem, enum base_types bt)
{
	assert (elem != NULL);

	memset(elem, 0, sizeof(struct element));
	elem->bt = bt;
	elem->next = NULL;
}

void
schema_element_print(element_t elem)
{
	int i;

	assert (elem != NULL);

	switch (elem->bt) {
	case CHARACTER:
		printf("CHAR(%d):[", elem->data.s.capacity);
		for (i = 0; i < elem->data.s.capacity; i++)
			printf("%c", elem->data.s.s[i]);
		printf("]");
		break;
	case VARCHAR:
		printf("VARCHAR(%d):[", elem->data.vs.capacity);
		for (i = 0; i < elem->data.vs.n; i++)
			printf("%c", elem->data.vs.s[i]);
		printf("]");
		break;
	case BOOLEAN:
		printf("BOOL:");
		if (elem->data.b == 0)
			printf("FALSE");
		else
			printf("TRUE");
		break;
	case INTEGER:
		printf("INT:%lld", elem->data.i);
		break;
	case FLOAT:
		printf("FLOAT:%4.2f", elem->data.f);
		break;
	case DOUBLE:
		printf("DOUBLE:%4.2f", elem->data.d);
		break;
	case DATE:
		printf("DATE:%02u/%02u/%04u",
		       elem->data.date.month,
		       elem->data.date.day,
		       elem->data.date.year);
		break;
	case TIME:
		printf("TIME:%02u:%02u:%02u",
		       elem->data.time.hours,
		       elem->data.time.minutes,
		       elem->data.time.seconds);
		printf("TIME");
		break;
	}
}

void
schema_element_insert(schema_t *s, element_t elem)
{
	assert (s != NULL);
	assert (elem != NULL);

	if (*s != NULL)
		elem->next = *s;

	*s = elem;
}

void
schema_element_remove(schema_t *s, element_t elem)
{
	schema_t prev, curr;

	assert (s != NULL);
	assert (elem != NULL);

	for (prev = NULL, curr = *s;
	     curr != NULL;
	     prev = curr, curr = curr->next)
		if (curr == elem) {
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
	element_t elem;

	for (elem = s; elem != NULL; elem = elem->next)
		schema_element_print(elem);
}
