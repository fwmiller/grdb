#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tuple.h"

#if _DEBUG
void bufdump(char *buf, int size);
#endif

static int
valid_date(char *val)
{
	char s[5];
	int d, m;

	/* Syntax check */
	if (!(isdigit(val[0]) &&
	    isdigit(val[1]) &&
	    val[2] == '-' &&
	    isdigit(val[3]) &&
	    isdigit(val[4]) &&
	    val[5] == '-' &&
	    isdigit(val[6]) &&
	    isdigit(val[7]) &&
	    isdigit(val[8]) &&
	    isdigit(val[9]))) {
#if _DEBUG
		printf("valid_date: bad syntax\n");
#endif
		return 0;
	}
	/* Month range check */
	memset(s, 0, 5);
	memcpy(s, val, 2);
	m = atoi(s);
	if (m < 1 || m > 12) {
#if _DEBUG
		printf("valid_date: month out of range\n");
#endif
		return 0;
	}
	/* Day of month range check */
	memset(s, 0, 5);
	memcpy(s, val + 3, 2);
	d = atoi(s);
	switch (m) {
	case 1: case 3: case 5: case 7: case 8: case 10: case 12:
		if (d < 1 || d > 31) {
#if _DEBUG
			printf("valid_date: day of month out of range\n");
#endif
			return 0;
		}
		break;

	case 4: case 6: case 9: case 11:
		if (d < 1 || d > 30) {
#if _DEBUG
			printf("valid_date: day of month out of range\n");
#endif
			return 0;
		}
		break;

	case 2:
		/* XXX Need to do better with leap year */
		if (d < 28 || d > 29) {
#if _DEBUG
			printf("valid_date: day of month out of range\n");
#endif
			return 0;
		}
		break;
	}

	/* Passed all checks */
	return 1;
}

static int
valid_time(char *val)
{
	char s[3];
	int h, m, sec;

	/* Syntax check */
	if (!(isdigit(val[0]) &&
	    isdigit(val[1]) &&
	    val[2] == ':' &&
	    isdigit(val[3]) &&
	    isdigit(val[4]) &&
	    val[5] == ':' &&
	    isdigit(val[6]) &&
	    isdigit(val[7]))) {
#if _DEBUG
		printf("valid_time: bad syntax\n");
#endif
		return 0;
	}
	/* Hours range check */
	memset(s, 0, 3);
	memcpy(s, val, 2);
	h = atoi(s);
	if (h < 0 || h > 24) {
#if _DEBUG
		printf("valid_time: hour out of range\n");
#endif
		return 0;
	}
	/* Minutes range check */
	memset(s, 0, 3);
	memcpy(s, val + 3, 2);
	m = atoi(s);
	if (m < 0 || m > 59) {
#if _DEBUG
		printf("valid_time: minutes out of range\n");
#endif
		return 0;
	}
	/* Seconds range check */
	memset(s, 0, 3);
	memcpy(s, val + 6, 2);
	sec = atoi(s);
	if (sec < 0 || sec > 59) {
#if _DEBUG
		printf("valid_time: seconds out of range\n");
#endif
		return 0;
	}
	return 1;
}

void tuple_set_enum(
        tuple_t t,
        char *attrname,
        char *type,
        char *val,
        enum_list_t el)
{
	enum_t e;
	int eidx, iidx, offset;

	eidx = enum_list_find_idx_by_name(el, type);
	if (eidx < 0) {
		printf("enum %s index not found\n", type);
		return;
	}
	e = enum_list_find_by_name(el, type);
	if (e == NULL) {
		printf("enum %s not found\n", attrname);
		return;
	}
	/* Get the index of the enum value */
	iidx = enum_find_idx_by_name(e, val);
	if (iidx < 0) {
		printf("enum %s does not contain %s\n", type, val);
		return;
	}
#if _DEBUG
	printf("%s found in enum %s at index %d\n", val, e->name, iidx);
#endif

	/* Offset of where the enum field is in the tuple buffer */
	offset = tuple_get_offset(t, attrname);
#if _DEBUG
	printf("attribute %s offset %d\n", attrname, offset);
#endif
	if (offset < 0) {
		printf("offset of %s not found\n", attrname);
		return;
	}
	tuple_set_char(t->buf + offset, (unsigned char) eidx);
	tuple_set_char(t->buf + offset + 1, (unsigned char) iidx);
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
#if _DEBUG
	printf("name [%s] offset %d\n", name, offset);
#endif
	if (offset < 0) {
		printf("offset of %s not found\n", name);
		return (-1);
	}
	bt = schema_find_type_by_name(t->s, name);
	if (bt == BASE_TYPES_MAX) {
		printf("type of %s not found\n", name);
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
		else if (strcasecmp(val, "false") == 0)
			tuple_set_bool(t->buf + offset, 0);

		break;

	/* case ENUM: handled elsewhere */

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
		if (valid_date(val))
			tuple_set_date(t->buf + offset, val);
		break;
	case TIME:
		if (valid_time(val))
			tuple_set_time(t->buf + offset, val);
		break;

	case ENUM:
		/* Handled elsewhere */
	case BASE_TYPES_MAX:
		break;
	}
#if _DEBUG
	bufdump(t->buf, t->len);
#endif
	return 0;
}
