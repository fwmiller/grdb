#include <assert.h>
#include <stdio.h>
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
	attribute_t attr;
	int offset = 0;

	assert (t != NULL);
	assert (name != NULL);

	for (attr = t->s; attr != NULL; attr = attr->next) {
		if (strcmp(name, attr->name) == 0)
			return offset;
		offset += base_types_len[attr->bt];
	}
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
	int offset;

	assert (t != NULL);

	printf("(");

	for (attr = t->s; attr != NULL; attr = attr->next) {
		offset = tuple_get_offset(t, attr->name);

		switch (attr->bt) {
		case CHARACTER:
			{
				int i = 0;

				for (; i < base_types_len[CHARACTER]; i++)
					printf("%c", *((char *)
						(t->buf + offset + i)));
			}
			break;

		case VARCHAR:
			printf("%s", (char *) (t->buf + offset));
			break;

		case BOOLEAN:
			{
				unsigned char val;
				val = *((unsigned char *)
					(t->buf + offset));
				if (val == 0)
					printf("FALSE");
				else
					printf("TRUE");
			}
			break;

		case INTEGER:
			{
				long long int val;
				val = *((long long int *) (t->buf + offset));
				printf("%lld", val);
			}
			break;

		case FLOAT:
			{
				float val;
				val = *((float *) (t->buf + offset));
				printf("%4.2f", val);
			}
			break;

		case DOUBLE:
			{
				double val;
				val = *((double *) (t->buf + offset));
				printf("%4.2f", val);
			}
			break;

		case DATE:
			{
				int i = 0;

				for (; i < base_types_len[DATE]; i++)
					printf("%c", *((char *)
						(t->buf + offset + i)));
			}
			break;

		case TIME:
			{
				int i = 0;

				for (; i < base_types_len[TIME]; i++)
					printf("%c", *((char *)
						(t->buf + offset + i)));
			}

		case BASE_TYPES_MAX:
			break;
		}

		if (attr->next != NULL)
			printf(",");

	}
	printf(")");
}
