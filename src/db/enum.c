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
#include "enum.h"
#include "types.h"


void
enum_init(enum_t *e)
{
	assert (e != NULL);

	if (*e != NULL)
		free(*e);

	*e = malloc(sizeof(struct grdb_enum));
	assert(*e != NULL);

	string_pool_init(&((*e)->pool));
	(*e)->next = NULL;
}

int
enum_file_open(char *grdbdir, int gidx, int cidx)
{
	char s[BUFSIZE];
	int fd;

	/* Create component enum file */
	memset(s, 0, BUFSIZE);
	sprintf(s, "%s/%d/%d/enum", grdbdir, gidx, cidx);
#if _DEBUG
	printf("enum_file_open: open enum file %s\n", s);
#endif
	fd = open(s, O_RDWR | O_CREAT, 0644);
#if _DEBUG
	if (fd < 0)
		printf("enum_file_open: open enum file failed (%s)\n",
			strerror(errno));
#endif
	return fd;
}

void
enum_print(enum_t e)
{
	string_pool_print(e->pool);
}

void
enum_insert(enum_t *e, char *s)
{
	assert (e != NULL);
	assert (*e != NULL);
	assert (s != NULL);

	string_pool_insert(&((*e)->pool), s);
}

char *enum_find_by_idx(enum_t e, int idx)
{
	return string_pool_find_by_idx(e->pool, idx);
}

int
enum_find_idx_by_name(enum_t e, char *s)
{
	return string_pool_find_idx_by_name(e->pool, s);
}

void
enum_set_name(enum_t e, char *name)
{
	assert (e != NULL);
	assert (name != NULL);

	strncpy(e->name, name, ENUM_NAME_LEN - 1);
}

char *
enum_get_name_ptr(enum_t e)
{
	assert (e != NULL);

	return e->name;
}

void
enum_list_init(enum_list_t *el)
{
	assert (el != NULL);
	*el = NULL;
}

int
enum_list_count(enum_list_t el)
{
	enum_t e;
	int n;

	assert (el != NULL);

	for (e = el, n = 0; e != NULL; e = e->next, n++);
	return n;
}

void
enum_list_print(enum_list_t el)
{
	enum_t e;

	for (e = el; e != NULL; e = e->next)
		enum_print(e);
}

void
enum_list_insert(enum_list_t *el, enum_t e)
{
	assert (el != NULL);
	assert (e != NULL);

	if (*el != NULL)
		e->next = *el;
	*el = e;
}

enum_t
enum_list_find_by_name(enum_list_t el, char *name)
{
	enum_t e;

	for (e = el; e != NULL; e = e->next)
		if (strcasecmp(name, e->name) == 0)
			return e;
	return NULL;
}

int
enum_list_find_idx_by_name(enum_list_t el, char *name)
{
	enum_t e;
	int i;

	for (i = 0, e = el; e != NULL; i++, e = e->next)
		if (strcasecmp(name, e->name) == 0)
			return i;
	return (-1);
}

enum_t
enum_list_find_by_idx(enum_list_t el, int idx)
{
	enum_t e;
	int i;

	for (i = 0, e = el; e != NULL; i++, e = e->next)
		if (i == idx)
			return e;

	return NULL;
}

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
