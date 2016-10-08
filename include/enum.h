#ifndef __ENUMERATIO_H
#define __ENUMERATIO_H

#include "string_pool.h"

typedef string_pool_t enum_t;

void enum_init(enum_t e);
void enum_print(enum_t e);
void enum_insert(enum_t e, char *s);

#endif
