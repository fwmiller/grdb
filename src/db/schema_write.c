#include <assert.h>
#if _DEBUG
#include <stdio.h>
#endif
#include <stdlib.h>
#include <unistd.h>
#include "schema.h"
#include "types.h"

schema_t
schema_write(schema_t s, int fd, int append)
{
	attribute_t attr;
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
	if (!append)
		lseek(fd, 0, SEEK_SET);
	len = write(fd, &n, sizeof(u64_t));
	if (len < sizeof(u64_t))
		return NULL;

	/* Write each attribute in the list */
	for (attr = s->attrlist; attr != NULL; attr = attr->next) {
		/* Write attribute name */
#if _DEBUG
		printf("schema_write: attribute name [%s]\n", attr->name);
#endif
		len = write(fd, attr->name, ATTR_NAME_MAXLEN);
		if (len < ATTR_NAME_MAXLEN)
			return NULL;

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

		/* Write attribute and enum names if attribute is an enum */
		if (attr->bt == ENUM) {
#if _DEBUG
			printf("schema_write: ");
			printf("enum name [%s]\n", attr->e->name);
#endif
			len = write(fd, attr->e->name, ENUM_NAME_LEN);
			if (len < ENUM_NAME_LEN)
				return NULL;
		}
	}
	return s;
}
