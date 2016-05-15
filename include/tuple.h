#ifndef __TUPLE_H
#define __TUPLE_H

#include "schema.h"

struct tuple {
	schema_t s;
	int len;
	void *buf;
};

typedef struct tuple *tuple_t;

#endif
