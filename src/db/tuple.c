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
	for (offset = 0, attr = s;
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
				char *date = "01-01-1970";
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

	for (attr = t->s, offset = 0;
	     attr != NULL;
	     offset += base_types_len[attr->bt], attr = attr->next)
		if (strcmp(name, attr->name) == 0)
			return offset;

	return (-1);
}

int
tuple_set(tuple_t t, char *name, char *val)
{
	int offset;
	base_types_t bt;
	
	assert (t != NULL);
	assert (name != NULL);
	assert (val != NULL);

	offset = tuple_get_offset(t, name);

	printf("name [%s] offset %d\n", name, offset);

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

void tuple_print(tuple_t t)
{
	attribute_t attr;
	int i, offset, val;
	float fval;
	double dval;
	unsigned char ch;

	assert (t != NULL);
	assert (t->buf != NULL);

	printf("[");

	for (attr = t->s; attr != NULL; attr = attr->next) {
		offset = tuple_get_offset(t, attr->name);
		if (offset >= 0) {
			switch (attr->bt) {
			case CHARACTER:
				ch = tuple_get_char(t->buf + offset);
				printf("'%c'", ch);
				break;

			case VARCHAR:
				printf("\"%s\"", (char *) (t->buf + offset));
				break;

			case BOOLEAN:
				val = tuple_get_bool(t->buf + offset);
				if (val == 0)
					printf("FALSE");
				else
					printf("TRUE");
				break;

			case INTEGER:
				i = tuple_get_int(t->buf + offset);
				printf("%d", i);
				break;

			case FLOAT:
				fval = tuple_get_float(t->buf + offset);
				printf("%4.2f", fval);
				break;

			case DOUBLE:
				dval = tuple_get_double(t->buf + offset);
				printf("%4.2f", dval);
				break;

			case DATE:
				{
					char s[base_types_len[DATE] + 1];

					memset(s, 0,
						base_types_len[DATE] + 1);
					tuple_get_date(t->buf + offset, s);
					printf("%s", s);
				}
				break;

			case TIME:
				{
					char s[base_types_len[TIME] + 1];

					memset(s, 0,
						base_types_len[TIME] + 1);
					tuple_get_time(t->buf + offset, s);
					printf("%s", s);
				}
				break;

			case BASE_TYPES_MAX:
				break;
			}
		}

		if (attr->next != NULL)
			printf(",");

	}
	printf("]");
}
