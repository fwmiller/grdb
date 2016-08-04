#include <assert.h>
#include <float.h>
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

/****************************************************************************
 * Getters and Setters - C style!
 */

static char
tuple_get_char(char *tbuf)
{
	return *tbuf;
}

static void
tuple_set_char(char *tbuf, char ch)
{
	assert (tbuf != NULL);
	*tbuf = ch;
}

static char *
tuple_get_varchar(char *tbuf, char *s, int maxlen)
{
	assert (tbuf != NULL);
	assert (s != NULL);

	memset(s, 0, maxlen);
	if (strlen(tbuf) < maxlen - 1)
		memcpy(s, tbuf, strlen(tbuf));
	else
		memcpy(s, tbuf, maxlen - 1);

	return s;
}

static void
tuple_set_varchar(char *tbuf, char *s)
{
	assert (tbuf != NULL);
	assert (s != NULL);

	memset(tbuf, 0, base_types_len[VARCHAR]);
	if (strlen(s) < base_types_len[VARCHAR] - 1)
		memcpy(tbuf, s, strlen(tbuf));
	else
		memcpy(tbuf, s, base_types_len[VARCHAR]);
}

static int
tuple_get_bool(char *tbuf)
{
	assert (tbuf != NULL);
	return (int) *((unsigned char *) tbuf);
}

static void
tuple_set_bool(char *tbuf, int val)
{
	assert (tbuf != NULL);
	*((unsigned char *) tbuf) = (unsigned char) val;
}

/***************************************************************************/

int
tuple_set(tuple_t t, char *name, char *val)
{
	int offset;
	base_types_t bt;
	
	assert (t != NULL);
	assert (name != NULL);
	assert (val != NULL);

	offset = tuple_get_offset(t, name);
	if (offset < 0) {
		printf("offset of [%s] not found\n", name);
		return (-1);
	}
	printf("offset of [%s] is %d\n", name, offset);

	bt = schema_find_type_by_name(t->s, name);
	if (bt == BASE_TYPES_MAX) {
		printf("type of [%s] not found\n", name);
		return (-1);
	}
	printf("type of [%s] is %s len %d\n",
		name, base_types_str[bt], base_types_len[bt]);

	switch (bt) {
	case CHARACTER:
		printf("set [%s] to '%c'\n", name, *((char *) val));
		memcpy(t->buf + offset, val, 1);
		printf("result '%c'\n", *((char *) (t->buf + offset)));
		break;

	case VARCHAR:
		printf("set [%s] to [%s]\n", name, val);
		memcpy(t->buf + offset, val, strlen(val));
		printf("result [%s]\n", (char *) t->buf + offset);
		break;

	case BOOLEAN:
		if (strcasecmp(val, "true") == 0) {
			unsigned char i = 1;
			printf("set [%s] to %d\n", name, (int) i);
			memcpy(t->buf + offset, &i, base_types_len[bt]);
			printf("result %d\n",
				(int) *((unsigned char *) t->buf + offset));
		} else {
			int i = 0;
			printf("set [%s] to %d\n", name, i);
			memcpy(t->buf + offset, &i, base_types_len[bt]);
			printf("result %d\n",
				(int) *((unsigned char *) t->buf + offset));
		}
		break;
	case INTEGER:
		{
			int i = atoi(val);
			printf("set [%s] to %d\n", name, i);
			memcpy(t->buf + offset, &i, base_types_len[bt]);
			printf("result %d\n", *((int *) t->buf + offset));
		}
		break;
	case FLOAT:
		{
			float fval = atof(val);
			printf("set [%s] to %4.2f\n", name, fval);
			memcpy(t->buf + offset, &fval, base_types_len[bt]);
			printf("result %4.2f\n", *((float *) t->buf + offset));
		}
		break;
	case DOUBLE:
		{
			double dval = atof(val);
			printf("set [%s] to %4.2f\n", name, dval);
			memcpy(t->buf + offset, &dval, base_types_len[bt]);
			printf("result %4.2f\n", *((double *) t->buf + offset));
		}
		break;
	case DATE:
	case TIME:
		memcpy(t->buf + offset, val, base_types_len[bt]);
		break;
	case BASE_TYPES_MAX:
		break;
	}
	memcpy(t->buf + offset, val, base_types_len[bt]);

	return 0;
}

void tuple_print(tuple_t t)
{
	attribute_t attr;
	int i, offset;
	float fval;
	double dval;
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
				i = *((int *) (t->buf + offset));
				printf("%d", i);
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
