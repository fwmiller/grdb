#if _DEBUG
#include <stdio.h>
#endif
#include <stdlib.h>
#include <unistd.h>
#include "enum.h"
#include "types.h"

enum_list_t
enum_list_write(enum_list_t el, int fd)
{
	enum_t e;
	u64_t n;
	int blen;
	ssize_t len;

	n = enum_list_count(el);
#if _DEBUG
	printf("enum_list_write: enum list has %llu entries\n", n);
#endif
	/* Write number of enums in enum list */
	len = write(fd, &n, sizeof(u64_t));
	if (len < sizeof(u64_t))
		return NULL;

	/* Write each enum in the list */
	for (e = el; e != NULL; e = e->next) {
		/* Write enum name */
#if _DEBUG
		printf("enum_list_write: enum name [%s]\n", e->name);
#endif
		len = write(fd, e->name, ENUM_NAME_LEN);
		if (len < ENUM_NAME_LEN)
			return NULL;
		/* Write enum string pool */
		blen = string_pool_overall_len(e->pool);
		len = write(fd, e->pool, blen);
		if (len < blen)
			return NULL;
	}
	return el;
}
