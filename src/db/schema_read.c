#include <assert.h>
#if _DEBUG
#include <stdio.h>
#endif
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "schema.h"
#include "types.h"

schema_t
schema_read(int fd, enum_list_t el)
{
	schema_t s = NULL;
	attribute_t attr = NULL;
	ssize_t len;
	u64_t n;
	u8_t ch;
	int i;

	/* Read number of attributes in schema */
	lseek(fd, 0, SEEK_SET);
	len = read(fd, &n, sizeof(u64_t));
	if (len < sizeof(u64_t))
		return NULL;
#if _DEBUG
	printf("schema_read: schema has %llu attribute", n);
	if (n >= 2)
		printf("s");
	printf("\n");
#endif
	schema_init(&s);

	/* Read each attribute in the schema */
	for (i = 0; i < n; i++) {
		attr = (attribute_t) malloc(sizeof(struct attribute));
		if (attr == NULL)
			return NULL;

		/* Read attribute name */
		len = read(fd, &(attr->name), ATTR_NAME_MAXLEN);
		if (len < ATTR_NAME_MAXLEN)
			return NULL;
#if _DEBUG
		printf("schema_read: attribute name [%s]\n", attr->name);
#endif

		/* Read attribute base type */
		len = read(fd, &ch, 1);
		if (len < 1)
			return NULL;
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
#if _DEBUG
			printf("schema_read: enum name [%s]\n", s);
#endif
			/* Lookup enum in enum list */
			e = enum_list_find_by_name(el, s);

			/* Hook the enum to the attribute if found */
			if (e == NULL) {
#if _DEBUG
				printf("schema_read: enum not found\n");
#endif
			} else {
				attr->e = e;
			}
		}
		/* Insert attribute in schema */
		schema_attribute_insert(s, attr);
		attr = NULL;
	}
	return s;
}
