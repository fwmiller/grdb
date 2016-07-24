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

	memset(t, 0, sizeof(struct tuple));
	t->s = s;
	t->len = schema_size(s);
	t->buf = malloc(t->len);
	assert(t->buf != NULL);
	memset(t->buf, 0, t->len);
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
	     attr = attr->next, offset += base_types_len[attr->bt])
		if (strcmp(name, attr->name) == 0)
			return offset;

	return (-1);
}

int
tuple_set(tuple_t t, char *name, void *val)
{
#if 0
	int offset;

	assert (t != NULL);
	assert (name != NULL);
	assert (val != NULL);

	offset = tuple_get_offset(t, name);
	if (offset < 0)
		return (-1);

	printf("tuple_set: offset %d len %d\n",
		offset, base_types_len[bt]);
	memcpy(t->buf + offset, val, base_types_len[bt]);
#endif
	return 0;
}

void tuple_print(tuple_t t)
{
	attribute_t attr;
	int i, offset;
	float fval;
	double dval;
	long long int lval;
	unsigned char val;

	assert (t != NULL);
	assert (t->buf != NULL);

	printf("[");

	for (attr = t->s; attr != NULL; attr = attr->next) {
		offset = tuple_get_offset(t, attr->name);
		if (offset >= 0) {
			switch (attr->bt) {
			case CHARACTER:
				for (i = 0;
				     i < base_types_len[CHARACTER];
				     i++)
					printf("%c", *((char *)
						(t->buf + offset + i)));
				break;

			case VARCHAR:
				printf("%s", (char *) (t->buf + offset));
				break;

			case BOOLEAN:
				val = *((unsigned char *) (t->buf + offset));
				if (val == 0)
					printf("FALSE");
				else
					printf("TRUE");
				break;

			case INTEGER:
				lval = *((long long int *) (t->buf + offset));
				printf("%lld", lval);
				break;

			case FLOAT:
				fval = *((float *) (t->buf + offset));
				printf("%4.2f", fval);
				break;

			case DOUBLE:
				dval = *((double *) (t->buf + offset));
				printf("%4.2f", dval);
				break;

			case DATE:
				for (i = 0; i < base_types_len[DATE]; i++)
					printf("%c", *((char *)
						(t->buf + offset + i)));
				break;

			case TIME:
				for (i = 0; i < base_types_len[TIME]; i++)
					printf("%c", *((char *)
						(t->buf + offset + i)));

			case BASE_TYPES_MAX:
				break;
			}
		}

		if (attr->next != NULL)
			printf(",");

	}
	printf("]");
}
