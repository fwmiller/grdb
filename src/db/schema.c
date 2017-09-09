#include <assert.h>
#if _DEBUG
#include <errno.h>
#endif
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "config.h"
#include "schema.h"
#include "types.h"

char *base_types_str[] = {
	"CHAR",
	"VARCHAR",
	"BOOL",
	"ENUM",
	"INT",
	"FLOAT",
	"DOUBLE",
	"DATE",
	"TIME"
};

int base_types_len[] = {
	1,			/* Single char */
	256,			/* Max string length */
	1,			/* Unsigned char 0=FALSE 1=TRUE */
	2,			/* Two unsigned chars 1) enum index */
				/*   2) item index */
	8,			/* For 64-bit */
	sizeof(float),		/* Should be an IEEE standard length */
	sizeof(double),		/* Should be an IEEE standard length */
	10,			/* "MM-DD-YYYY" string no terminator */
	8			/* "HH:MM:SE" string no terminator */
};

void
schema_attribute_init(
	attribute_t attr,
	void *name,
	enum base_types bt,
	enum_t e)
{
	assert (attr != NULL);
	assert (name != NULL);

	memset(attr, 0, sizeof(struct attribute));
	strncpy(attr->name, name, ATTR_NAME_MAXLEN - 1);
	attr->bt = bt;
	attr->e = e;
	attr->next = NULL;
}

void
schema_attribute_print(attribute_t attr, enum_list_t el)
{
	assert (attr != NULL);

	printf("%s:%s",
	       (attr->bt == ENUM ?
		attr->e->name :
		base_types_str[attr->bt]),
	       attr->name);
}

void
schema_attribute_insert(schema_t s, attribute_t attr)
{
	attribute_t t;

	assert (s != NULL);
	assert (attr != NULL);

	if (s->attrlist == NULL) {
		s->attrlist = attr;
		return;
	}
	for (t = s->attrlist; t->next != NULL; t = t->next);
	t->next = attr;
}

void
schema_attribute_remove(schema_t s, attribute_t attr)
{
	attribute_t prev, curr;

	assert (s != NULL);
	assert (attr != NULL);

	for (prev = NULL, curr = s->attrlist;
	     curr != NULL;
	     prev = curr, curr = curr->next)
		if (curr == attr) {
			if (prev == NULL) {
				s->attrlist = curr->next;
			} else {
				prev->next = curr->next;
			}
		}
}

void
schema_init(schema_t *s)
{
	assert (s != NULL);
#if 0
	/* XXX Not having something like this could be a memory leak */
	if (*s != NULL)
		free(s);
#endif
	*s = malloc(sizeof(struct schema));
	memset(*s, 0, sizeof(struct schema));
}

int
schema_file_init(int gidx, int cidx, char *name)
{
	char s[BUFSIZE];
	int fd;

	/* Create component schema file */
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/%s", GRDBDIR, gidx, cidx, name);
#if _DEBUG
	printf("schema_file_init: open schema file %s\n", s);
#endif
	fd = open(s, O_RDWR | O_CREAT, 0644);
#if _DEBUG
	if (fd < 0)
		printf("schema_file_init: open schema file failed (%s)\n",
			strerror(errno));
#endif
	return fd;
}

int
schema_count(schema_t s)
{
	attribute_t attr;
	int cnt;

	assert(s != NULL);

	for (attr = s->attrlist, cnt = 0;
	     attr != NULL;
	     attr = attr->next, cnt++);

	return cnt;
}

int
schema_size(schema_t s)
{
	attribute_t attr;
	int acc = 0;

	assert(s != NULL);

	for (attr = s->attrlist; attr != NULL; attr = attr->next)
		acc += base_types_len[attr->bt];
	return acc;
}

base_types_t
schema_find_type_by_name(schema_t s, char *name)
{
	attribute_t attr;

	assert (s != NULL);

#if _DEBUG
	printf("find schema [%s]\n", name);
#endif
	for (attr = s->attrlist; attr != NULL; attr = attr->next)
		if (strcasecmp(name, attr->name) == 0)
			return attr->bt;

	return BASE_TYPES_MAX;
}

attribute_t
schema_find_attr_by_name(schema_t s, char *name)
{
	attribute_t attr;

	assert (s != NULL);

#if _DEBUG
	printf("find attribute [%s]\n", name);
#endif
	for (attr = s->attrlist; attr != NULL; attr = attr->next)
		if (strcasecmp(name, attr->name) == 0)
			return attr;

	return NULL;
}

void
schema_print(schema_t s, enum_list_t el)
{
	attribute_t attr;

	assert (s != NULL);

	printf("[");
	for (attr = s->attrlist; attr != NULL; attr = attr->next) {
		schema_attribute_print(attr, el);
		if (attr->next != NULL)
			printf(",");
	}
	printf("]");

	/* XXX Print enum list */
}

schema_t
schema_read(int fd, enum_list_t el)
{
	schema_t s = NULL;
	attribute_t attr = NULL;
	off_t off;
	ssize_t len;
	u64_t n;
	u8_t ch;
	int i;

	/* Read number of attributes in schema */
	off = 0;
	lseek(fd, off, SEEK_SET);
	len = read(fd, &n, sizeof(u64_t));
	if (len < sizeof(u64_t))
		return NULL;
	off += sizeof(u64_t);
#if _DEBUG
	printf("schema_read: schema has %llu attributes\n", n);
#endif
	schema_init(&s);

	/* Read each attribute in the schema */
	for (i = 0; i < n; i++) {
		attr = (attribute_t) malloc(sizeof(struct attribute));
		if (attr == NULL)
			return NULL;

		/* Read attribute name */
		lseek(fd, off, SEEK_SET);
		len = read(fd, &(attr->name), ATTR_NAME_MAXLEN);
		if (len < ATTR_NAME_MAXLEN)
			return NULL;
		off += ATTR_NAME_MAXLEN;
#if _DEBUG
		printf("schema_read: attribute name [%s]\n", attr->name);
#endif

		/* Read attribute base type */
		lseek(fd, off, SEEK_SET);
		len = read(fd, &ch, 1);
		if (len < 1)
			return NULL;
		off += 1;
		attr->bt = (enum base_types) ch;
#if _DEBUG
		printf("schema_read: attribute base type [%s]\n",
			base_types_str[attr->bt]);
#endif
		/* Read enum name if attribute is an enum */
		if (attr->bt == ENUM) {
			char s[ENUM_NAME_LEN];
			enum_t e;

			memset(s, 0, ENUM_NAME_LEN);
			len = read(fd, s, ENUM_NAME_LEN);
			if (len < ENUM_NAME_LEN)
				return NULL;
			off += ENUM_NAME_LEN;
#if _DEBUG
			printf("schema_read: enum name [%s]\n", s);
#endif
			/* Lookup enum in enum list */
			e = enum_list_find_by_name(el, s);

			/* Hook the enum to the attribute if found */
			if (e != NULL)
				attr->e = e;
		}
		/* Insert attribute in schema */
		schema_attribute_insert(s, attr);
		attr = NULL;
	}
	return s;
}

schema_t
schema_write(schema_t s, int fd)
{
	attribute_t attr;
	off_t off;
	ssize_t len;
	u64_t n;
	u8_t ch;

	assert (s != NULL);

	n = (u64_t) schema_count(s);
	if (n == 0)
		return NULL;

#if _DEBUG
	printf("schema_write: schema has %llu attributes\n", n);
#endif
	/* Write number of attributes in schema */
	off = 0;
	lseek(fd, off, SEEK_SET);
	len = write(fd, &n, sizeof(u64_t));
	if (len < sizeof(u64_t))
		return NULL;
	off += sizeof(u64_t);

	/* Write each attribute in the list */
	for (attr = s->attrlist; attr != NULL; attr = attr->next) {
		/* Write attribute name */
#if _DEBUG
		printf("schema_write: attribute name [%s]\n", attr->name);
#endif
		len = write(fd, attr->name, ATTR_NAME_MAXLEN);
		if (len < ATTR_NAME_MAXLEN)
			return NULL;
		off += ATTR_NAME_MAXLEN;

		/* Write attribute base type */
#if _DEBUG
		printf("schema_write: ");
		printf("attribute base type [%s]\n",
			base_types_str[attr->bt]);
#endif
		ch = (unsigned char) attr->bt;
		len = write(fd, &ch, 1);
		if (len < 1)
			return NULL;
		off += 1;

		/* Write attribute and enum names if attribute is an enum */
		if (attr->bt == ENUM) {
#if _DEBUG
			printf("schema_write: ");
			printf("enum name [%s]\n", attr->e->name);
#endif
			len = write(fd, attr->e->name, ENUM_NAME_LEN);
			if (len < ENUM_NAME_LEN)
				return NULL;
			off += ENUM_NAME_LEN;
		}
	}
	return s;
}
