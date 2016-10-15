#include <assert.h>
#include <stdlib.h>
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

void
enum_print(enum_t e)
{
	string_pool_print(e->pool);
}

void
enum_insert(enum_t *e, char *s)
{
	assert (e != NULL);
	assert (*e != NULL);
	assert (s != NULL);

	string_pool_insert(&((*e)->pool), s);
}

void
enum_list_init(enum_list_t *el)
{
	assert (el != NULL);
	*el = NULL;
}

void
enum_list_print(enum_list_t el)
{
	enum_t e;

	assert (el != NULL);

	for (e = el; e != NULL; e = e->next)
		enum_print(e);
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
