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
tuple_init(tuple_t *t, schema_t s)
{
	attribute_t attr;
	int offset;

	if (s == NULL) {
#if _DEBUG
		printf("tuple_init: missing schema\n");
#endif
		return;
	}
	if (t == NULL) {
#if _DEBUG
		printf("tuple_init: missing tuple\n");
#endif
		return;
	}
	if (*t == NULL)
		*t = malloc(sizeof(struct tuple));
	memset(*t, 0, sizeof(struct tuple));
	(*t)->s = s;
	(*t)->len = schema_size(s);
	(*t)->buf = malloc((*t)->len);
	assert((*t)->buf != NULL);
	memset((*t)->buf, 0, (*t)->len);

	/* Set inital values */
	for (offset = 0, attr = s->attrlist;
	     attr != NULL;
	     offset += base_types_len[attr->bt], attr = attr->next)
		tuple_set_default_value(attr->bt, (*t)->buf, offset);
}

void
tuple_set_default_value(enum base_types bt, char *buf, int offset)
{
	switch (bt) {
	case CHARACTER:
		tuple_set_char(buf + offset, 0);
		break;
	case VARCHAR:
		{
			char *s = "";
			tuple_set_varchar(buf + offset, s);
		}
		break;
	case BOOLEAN:
		tuple_set_bool(buf + offset, 0);
		break;
	case ENUM:
		tuple_set_char(buf + offset, 0);
		break;
	case INTEGER:
		tuple_set_int(buf + offset, 0);
		break;
	case FLOAT:
		tuple_set_float(buf + offset, 0.0);
		break;
	case DOUBLE:
		tuple_set_double(buf + offset, 0.0);
		break;
	case DATE:
		{
			/* A mark in time */
			char *date = "08-27-2016";
			//char *date = "01-01-1970";
			tuple_set_date(buf + offset, date);
		}
		break;
	case TIME:
		{
			char *time = "00:00:00";
			tuple_set_time(buf + offset, time);
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
#if 0
	if (t->buf != NULL)
		free(t->buf);
#endif
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
