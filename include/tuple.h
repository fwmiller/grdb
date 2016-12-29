#ifndef __TUPLE_H
#define __TUPLE_H

#include "schema.h"

struct tuple {
	schema_t s;
	int len;
	void *buf;
};

typedef struct tuple *tuple_t;

void tuple_init(tuple_t t, schema_t s);
void tuple_delete(tuple_t t);
int tuple_get_offset(tuple_t t, char *name);
int tuple_set(tuple_t t, char *name, char *val);

void tuple_set_enum(
	tuple_t t,
	char *attrname,
	char *type,
	char *val,
	enum_list_t el);

void tuple_print(tuple_t t, enum_list_t el);

/* Getters */
char   tuple_get_char(char *tbuf);
char  *tuple_get_varchar(char *tbuf, char *s, int maxlen);
int    tuple_get_bool(char *tbuf);
int    tuple_get_int(char *tbuf);
float  tuple_get_float(char *tbuf);
double tuple_get_double(char *tbuf);
char  *tuple_get_date(char *tbuf, char *date);
char  *tuple_get_time(char *tbuf, char *time);

/* Setters */
void tuple_set_char(char *tbuf, char ch);
void tuple_set_varchar(char *tbuf, char *s);
void tuple_set_bool(char *tbuf, int val);
void tuple_set_int(char *tbuf, int val);
void tuple_set_float(char *tbuf, float val);
void tuple_set_double(char *tbuf, double val);
void tuple_set_date(char *tbuf, char *date);
void tuple_set_time(char *tbuf, char *time);

#endif
