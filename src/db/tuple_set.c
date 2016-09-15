#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tuple.h"

int
tuple_set(tuple_t t, char *name, char *val)
{
	int offset;
	base_types_t bt;
	
	assert (t != NULL);
	assert (name != NULL);
	assert (val != NULL);

	offset = tuple_get_offset(t, name);
#if _DEBUG
	printf("name [%s] offset %d\n", name, offset);
#endif
	if (offset < 0) {
		printf("offset of [%s] not found\n", name);
		return (-1);
	}
	bt = schema_find_type_by_name(t->s, name);
	if (bt == BASE_TYPES_MAX) {
		printf("type of [%s] not found\n", name);
		return (-1);
	}
	switch (bt) {
	case CHARACTER:
		tuple_set_char(t->buf + offset, val[0]);
		break;

	case VARCHAR:
		tuple_set_varchar(t->buf + offset, val);
		break;

	case BOOLEAN:
		if (strcasecmp(val, "true") == 0)
			tuple_set_bool(t->buf + offset, 1);
		else
			tuple_set_bool(t->buf + offset, 0);

		break;
	case INTEGER:
		{
			int i = atoi(val);
			tuple_set_int(t->buf + offset, i);
		}
		break;
	case FLOAT:
		{
			float fval = atof(val);
			tuple_set_float(t->buf + offset, fval);
		}
		break;
	case DOUBLE:
		{
			double dval = atof(val);
			tuple_set_double(t->buf + offset, dval);
		}
		break;
	case DATE:
		/* XXX Need to validate date string */
		tuple_set_date(t->buf + offset, val);
		break;
	case TIME:
		/* XXX Need to validate time string */
		tuple_set_time(t->buf + offset, val);
		break;
	case BASE_TYPES_MAX:
		break;
	}
#if _DEBUG
	bufdump(t->buf, t->len);
#endif
	return 0;
}
