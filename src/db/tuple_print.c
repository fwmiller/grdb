#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "enum.h"
#include "tuple.h"

static void
tuple_print_enum(tuple_t t, int offset, enum_list_t el)
{
	int eidx, iidx;
	enum_t e;
	char *s;

	eidx = (int) tuple_get_char(t->buf + offset);
	iidx = (int) tuple_get_char(t->buf + offset + 1);

	e = enum_list_find_by_idx(el, eidx);
	if (e == NULL)
		return;

	s = enum_find_by_idx(e, iidx);
	printf("%s", s);
}

void
tuple_print(tuple_t t, enum_list_t el)
{
	attribute_t attr;
	int i, offset, val;
	float fval;
	double dval;

	assert (t != NULL);
	assert (t->buf != NULL);

	printf("[");

	for (attr = t->s->attrlist; attr != NULL; attr = attr->next) {
		offset = tuple_get_offset(t, attr->name);
		if (offset >= 0) {
			switch (attr->bt) {
			case CHARACTER:
				printf("'%c'",
					tuple_get_char(t->buf + offset));
				break;

			case VARCHAR:
				printf("\"%s\"",
					(char *) (t->buf + offset));
				break;

			case BOOLEAN:
				val = tuple_get_bool(t->buf + offset);
				if (val == 0)
					printf("FALSE");
				else
					printf("TRUE");
				break;

			case ENUM:
				tuple_print_enum(t, offset, el);
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
