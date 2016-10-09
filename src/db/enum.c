#include "enum.h"


void
enum_init(enum_t *e)
{
	string_pool_init(e);
}

void
enum_print(enum_t e)
{
	string_pool_print(e);
}

void
enum_insert(enum_t *e, char *s)
{
	string_pool_insert(e, s);
}
