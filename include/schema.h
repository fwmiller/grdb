#ifndef __SCHEMA_H
#define __SCHEMA_H


#define ATTR_NAME_MAXLEN	256

/* Base types */
enum base_types
{
	CHARACTER,	/* Fixed length character string */
	VARCHAR,	/* Variable length character string */
	BOOLEAN,	/* Boolean */
	INTEGER,	/* Integer (64-bit integer) */
	FLOAT,		/* Float */
	DOUBLE,		/* Double */
	DATE,		/* Date */
	TIME,		/* Time (24 hour format) */
	BASE_TYPES_MAX
};

struct attribute {
	char name[ATTR_NAME_MAXLEN];
	enum base_types bt;
	struct attribute *next;
};

typedef struct attribute *attribute_t;
typedef struct attribute *schema_t;

extern char *base_types_str[];

void schema_attribute_init(attribute_t attr, enum base_types bt, char *name);
void schema_attribute_print(attribute_t attr);

void schema_attribute_insert(schema_t *s, attribute_t attr);
void schema_attribute_remove(schema_t *s, attribute_t attr);
int schema_size(schema_t s);
void schema_print(schema_t s);


#endif
