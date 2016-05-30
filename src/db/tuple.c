#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "tuple.h"

void
tuple_init(tuple_t t, schema_t s)
{
	assert (t != NULL);
	assert (s != NULL);

	t->s = s;
	t->len = schema_size(s);
	if (t->buf != NULL)
		free(t->buf);
	t->buf = malloc(t->len);
	assert(t->buf != NULL);
	memset(t->buf, 0, t->len);
}

static int
tuple_get_offset(tuple_t t, char *name)
{
	return (-1);
}

void
tuple_set(tuple_t t, base_types_t bt, char *name, void *val)
{
	int offset;

	assert (t != NULL);
	assert (name != NULL);
	assert (val != NULL);

	offset = tuple_get_offset(t, name);
	if (offset >= 0)
		memcpy(t->buf + offset, val, base_types_len[bt]);
}

void tuple_print(tuple_t t)
{
	attribute_t attr;

	assert (t != NULL);

	for (attr = t->s; attr != NULL; attr = attr->next) {
		switch (attr->bt) {
		case CHARACTER:
			break;
		case VARCHAR:
			break;
		case BOOLEAN:
			break;
		case INTEGER:
			break;
		case FLOAT:
			break;
		case DOUBLE:
			break;
		case DATE:
			break;
		case TIME:
		case BASE_TYPES_MAX:
			break;
		}
	}
}
