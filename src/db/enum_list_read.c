#if _DEBUG
#include <stdio.h>
#endif
#include <stdlib.h>
#include <unistd.h>
#include "enum.h"
#include "types.h"

enum_list_t
enum_list_read(enum_list_t *el, int fd)
{
	enum_t e;
	off_t off;
	u64_t n;
	ssize_t len;
	int i;

	/* Read number of enums in enum list */
	off = 0;
	lseek(fd, off, SEEK_SET);
	len = read(fd, &n, sizeof(u64_t));
	if (len < sizeof(u64_t))
		return NULL;
	off += sizeof(u64_t);
#if _DEBUG
	printf("enum_list_read: enum list has %llu entries\n", n);
#endif

	/* Read each enum in the list */
	for (i = 0; i < n; i++, e = NULL) {
		char *buf = NULL;
		unsigned char entries;
		unsigned short memlen;
		int poollen;

		/* Read enum name */
		enum_init(&e);
		len = read(fd, &(e->name), ENUM_NAME_LEN);
		off += ENUM_NAME_LEN;
#if _DEBUG
		printf("enum_list_read: enum name [%s]\n", e->name);
#endif
		/*** Read enum string pool ***/

		/* Read number of entries */
		lseek(fd, off, SEEK_SET);
		len = read(fd, &entries, 1);
		if (len < 1)
			return NULL;
		off += 1;
#if _DEBUG
		printf("enum_list_read: ");
		printf("string pool entries %u\n", entries);
#endif
		if (entries == 0)
			return NULL;

		/* Read string pool memory length */
		lseek(fd, off, SEEK_SET);
		len = read(fd, &memlen, 2);
		if (len < 2)
			return NULL;
		off += 2;
#if _DEBUG
		printf("enum_list_read: ");
		printf("memory length %u\n", memlen);
#endif
		/* Read string pool entries array and pool memory */
		poollen = (entries << 1) + memlen;
		buf = malloc(3 + poollen);
		lseek(fd, off, SEEK_SET);
		len = read(fd, buf + 3, poollen);
		if (len < poollen)
			return NULL;
		off += poollen;

		*((unsigned char *) buf) = entries;
		*((unsigned short *) (buf + 1)) = memlen;
		e->pool = (string_pool_t) buf;

		enum_list_insert(el, e);
	}
	return *el;
}
