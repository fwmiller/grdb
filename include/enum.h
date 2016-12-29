#ifndef __ENUMERATION_H
#define __ENUMERATION_H

#include "string_pool.h"

#define ENUM_NAME_LEN	256

struct grdb_enum {
	char name[ENUM_NAME_LEN];
	string_pool_t pool;
	struct grdb_enum *next;
};

typedef struct grdb_enum *enum_t;
typedef enum_t enum_list_t;

void enum_init(enum_t *e);
void enum_print(enum_t e);
void enum_insert(enum_t *e, char *s);
char *enum_find_by_idx(enum_t e, int idx);
int enum_find_idx_by_name(enum_t e, char *s);

void enum_set_name(enum_t e, char *name);
char *enum_get_name_ptr(enum_t e);

void enum_list_init(enum_list_t *el);
void enum_list_print(enum_list_t el);
void enum_list_insert(enum_list_t *el, enum_t e);
enum_t enum_list_find_by_name(enum_list_t el, char *name);
int enum_list_find_idx_by_name(enum_list_t el, char *name);
enum_t enum_list_find_by_idx(enum_list_t el, int idx);

#endif
