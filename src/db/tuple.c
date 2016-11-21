#include <assert.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tuple.h"

#if _DEBUG
void bufdump(char *buf, int size);
#endif

void
tuple_init(tuple_t t, schema_t s)
{
	attribute_t attr;
	int offset;

	assert (t != NULL);
	assert (s != NULL);

	memset(t, 0, sizeof(struct tuple));
	t->s = s;
	t->len = schema_size(s);
	t->buf = malloc(t->len);
	assert(t->buf != NULL);
	memset(t->buf, 0, t->len);

	/* Set inital values */
	for (offset = 0, attr = s->attrlist;
	     attr != NULL;
	     offset += base_types_len[attr->bt], attr = attr->next)
		switch (attr->bt) {
		case CHARACTER:
			tuple_set_char(t->buf + offset, 0);
			break;
		case VARCHAR:
			{
				char *s = "";
				tuple_set_varchar(t->buf + offset, s);
			}
			break;
		case BOOLEAN:
			tuple_set_bool(t->buf + offset, 0);
			break;
		case ENUM:
			tuple_set_char(t->buf + offset, 0);
			break;
		case INTEGER:
			tuple_set_int(t->buf + offset, 0);
			break;
		case FLOAT:
			tuple_set_float(t->buf + offset, 0.0);
			break;
		case DOUBLE:
			tuple_set_double(t->buf + offset, 0.0);
			break;
		case DATE:
			{
				/* A mark in time */
				char *date = "08-27-2016";
				//char *date = "01-01-1970";
				tuple_set_date(t->buf + offset, date);
			}
			break;
		case TIME:
			{
				char *time = "00:00:00";
				tuple_set_time(t->buf + offset, time);
			}
			break;
		case BASE_TYPES_MAX:
			break;
		}
}

void
tuple_delete(tuple_t t)
{
	assert (t != NULL);

	if (t->buf != NULL)
		free(t->buf);

	memset(t, 0, sizeof(struct tuple));
}

int
tuple_get_offset(tuple_t t, char *name)
{
	attribute_t attr;
	int offset;

	assert (t != NULL);
	assert (name != NULL);

	for (attr = t->s->attrlist, offset = 0;
	     attr != NULL;
	     offset += base_types_len[attr->bt], attr = attr->next)
		if (strcmp(name, attr->name) == 0)
			return offset;

	return (-1);
}
