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

#endif
