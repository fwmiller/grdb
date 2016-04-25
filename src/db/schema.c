#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "schema.h"

void
schema_element_alloc(element_t elem, enum base_types bt)
{

}

void
schema_element_print(element_t elem)
{
	assert (elem != NULL);

	switch (elem->bt) {
	case CHARACTER:
		break;
	case VARCHAR:
		printf("VARCHAR");
		break;
	case BOOLEAN:
		printf("BOOL");
		break;
	case INTEGER:
		printf("INT");
		break;
	case FLOAT:
		printf("FLOAT");
		break;
	case DOUBLE:
		printf("DOUBLE");
		break;
	case DATE:
		printf("DATE");
		break;
	case TIME:
		printf("TIME");
		break;
	}
}

void
schema_element_insert(schema_t s, element_t elem)
{

}

void
schema_element_remove(schema_t s, element_t elem)
{

}

void
schema_print(schema_t s)
{

}
