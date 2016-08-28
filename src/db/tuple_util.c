#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tuple.h"


/****************************************************************************
 * Getters and Setters - C style!
 */

char
tuple_get_char(char *tbuf)
{
	return *tbuf;
}

void
tuple_set_char(char *tbuf, char ch)
{
	assert (tbuf != NULL);
	*tbuf = ch;
}

char *
tuple_get_varchar(char *tbuf, char *s, int maxlen)
{
	assert (tbuf != NULL);
	assert (s != NULL);

	memset(s, 0, maxlen);
	if (strlen(tbuf) < maxlen - 1)
		memcpy(s, tbuf, strlen(tbuf));
	else {
		printf("get varchar overflow\n");
		memcpy(s, tbuf, maxlen - 1);
	}
	return s;
}

void
tuple_set_varchar(char *tbuf, char *s)
{
	assert (tbuf != NULL);
	assert (s != NULL);

	memset(tbuf, 0, base_types_len[VARCHAR]);
	if (strlen(s) < base_types_len[VARCHAR] - 1)
		memcpy(tbuf, s, strlen(s));
	else {
		printf("set varchar overflow\n");
		memcpy(tbuf, s, base_types_len[VARCHAR] - 1);
	}
}

int
tuple_get_bool(char *tbuf)
{
	assert (tbuf != NULL);
	return (int) *((unsigned char *) tbuf);
}

void
tuple_set_bool(char *tbuf, int val)
{
	assert (tbuf != NULL);
	*((unsigned char *) tbuf) = (unsigned char) val;
}

int
tuple_get_int(char *tbuf)
{
	assert (tbuf != NULL);
	return *((int *) tbuf);
}

void
tuple_set_int(char *tbuf, int val)
{
	assert (tbuf != NULL);
	*((int *) tbuf) = val;
}

float
tuple_get_float(char *tbuf)
{
	assert (tbuf != NULL);
	return *((float *) tbuf);
}

void
tuple_set_float(char *tbuf, float val)
{
	assert (tbuf != NULL);
	*((float *) tbuf) = val;
}

double
tuple_get_double(char *tbuf)
{
	assert (tbuf != NULL);
	return *((double *) tbuf);
}

void
tuple_set_double(char *tbuf, double val)
{
	assert (tbuf != NULL);
	*((double *) tbuf) = val;
}

char *
tuple_get_date(char *tbuf, char *date)
{
	assert (tbuf != NULL);
	assert (date != NULL);

	memcpy(date, tbuf, base_types_len[DATE]);
	return date;
}

void
tuple_set_date(char *tbuf, char *date)
{
	assert (tbuf != NULL);
	assert (date != NULL);

	memcpy(tbuf, date, base_types_len[DATE]);
}

char *
tuple_get_time(char *tbuf, char *time)
{
	assert (tbuf != NULL);
	assert (time != NULL);

	memcpy(time, tbuf, base_types_len[TIME]);
	return time;
}

void
tuple_set_time(char *tbuf, char *time)
{
	assert (tbuf != NULL);
	assert (time != NULL);

	memcpy(tbuf, time, base_types_len[TIME]);
}
