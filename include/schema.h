#ifndef __SCHEMA_H
#define __SCHEMA_H


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
	TIME		/* Time (24 hour format) */
};

struct bt_char {
	int capacity;
	char *s;
};

struct bt_varchar {
	int capacity;
	int n;
	char *s;
};

struct bt_date {
	unsigned char month;
	unsigned char day;
	unsigned short year;
};

struct bt_time {
	unsigned char hours;
	unsigned char minutes;
	unsigned char seconds;
};

typedef struct bt_char *bt_char_t;
typedef struct bt_varchar *bt_varchar_t;
typedef int bt_boolean_t;
typedef long long int bt_integer_t;
typedef float bt_float_t;
typedef double bt_double_t;
typedef struct bt_date *bt_date_t;
typedef struct bt_time *bt_time_t;

union attribute {
	struct bt_char s;
	struct bt_varchar vs;
	bt_boolean_t b;
	bt_integer_t i;
	bt_float_t f;
	bt_double_t d;
	struct bt_date date;
	struct bt_time time;
};

struct element {
	enum base_types bt;
	union attribute data;
	struct element *next;
};

typedef union attribute *attribute_t;
typedef struct element *element_t;
typedef struct element *schema_t;


void schema_element_alloc(element_t elem, enum base_types bt);
void schema_element_print(element_t elem);

void schema_element_insert(schema_t s, element_t elem);
void schema_element_remove(schema_t s, element_t elem);
void schema_print(schema_t s);


#endif
