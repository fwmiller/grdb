#ifndef __SCHEMA_H
#define __SCHEMA_H


#include "enum.h"

#define ATTR_NAME_MAXLEN	256

/* Base types */
enum base_types
{
	CHARACTER,	/* Fixed length character string */
	VARCHAR,	/* Variable length character string */
	BOOLEAN,	/* Boolean */
	ENUM,		/* Enum type */
	INTEGER,	/* Integer */
	FLOAT,		/* Float */
	DOUBLE,		/* Double */
	DATE,		/* Date string */
	TIME,		/* Time string (24 hour format) */
	BASE_TYPES_MAX
};

struct attribute {
	char name[ATTR_NAME_MAXLEN];	/* Name */
	enum_t e;			/* enum_t if ENUM */
	enum base_types bt;		/* Base type */
	struct attribute *next;
};

struct schema {
	struct attribute *attrlist;		/* Attributes list */
};

typedef enum base_types base_types_t;
typedef struct attribute *attribute_t;
typedef struct schema *schema_t;

extern char *base_types_str[];
extern int base_types_len[];

void schema_attribute_init(
        attribute_t attr,
        void *name,
        enum base_types bt,
        enum_t e);

void schema_attribute_print(attribute_t attr);
void schema_attribute_insert(schema_t s, attribute_t attr);
void schema_attribute_remove(schema_t s, attribute_t attr);

void schema_init(schema_t *s);
int schema_size(schema_t s);
base_types_t schema_find_type_by_name(schema_t s, char *name);
attribute_t schema_find_attr_by_name(schema_t s, char *name);
void schema_print(schema_t s);


#endif
