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
	off_t off;
	u64_t n;
	int blen;
	ssize_t len;

	n = enum_list_count(el);
#if _DEBUG
	printf("enum_list_write: enum list has %llu entries\n", n);
#endif
	/* Write number of enums in enum list */
	off = 0;
	lseek(fd, off, SEEK_SET);
	len = write(fd, &n, sizeof(u64_t));
	if (len < sizeof(u64_t))
		return NULL;

	off += sizeof(u64_t);

	/* Write each enum in the list */
	for (e = el; e != NULL; e = e->next) {
		/* Write enum name */
#if _DEBUG
		printf("enum_list_write: enum name [%s]\n", e->name);
#endif
		len = write(fd, e->name, ENUM_NAME_LEN);
		if (len < ENUM_NAME_LEN)
			return NULL;
		off += ENUM_NAME_LEN;
#if _DEBUG
		printf("enum_list_write: string pool ");
		string_pool_print(e->pool);
#endif
		/* Write enum string pool */
		lseek(fd, off, SEEK_SET);
		blen = string_pool_overall_len(e->pool);
		len = write(fd, e->pool, blen);
		if (len < blen)
			return NULL;
		off += blen;
	}
	return el;
}
