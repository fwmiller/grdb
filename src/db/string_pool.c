#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string_pool.h"

static inline int
string_pool_get_entries(string_pool_t pool)
{
	return (int) (((unsigned char *) pool)[0]);
}

static inline void
string_pool_set_entries(string_pool_t pool, unsigned char n)
{
	pool[0] = n;
}

static inline int
string_pool_get_len(string_pool_t pool)
{
	return (int) *((unsigned short *) (((unsigned char *) pool) + 1));
}

static inline void
string_pool_set_len(string_pool_t pool, unsigned short len)
{
	*((unsigned short *) (((unsigned char *) pool) + 1)) = len;

}

static inline char *
string_pool_get_string(string_pool_t pool, int n, int i)
{
	unsigned short idx;
	unsigned char *pos;

	idx = *((unsigned short *) (pool + 3 + i * 2));
	pos = pool + 3 + n * 2;
	return (char *) (pos + idx);
}

void
string_pool_init(string_pool_t *pool)
{
	assert(pool != NULL);

	if (*pool != NULL)
		free(*pool);

	*pool = malloc(1);
	assert(*pool != NULL);
	string_pool_set_entries(*pool, 0);
}

void
string_pool_print(string_pool_t pool)
{
	int i, n;

	if (pool == NULL) {
#if _DEBUG
		printf("string_pool_init: pool is NULL\n");
#endif
		return;
	}
	/* n is the number of entries in the current string pool */
	n = string_pool_get_entries(pool);
	
#if _DEBUG
	printf("entries %d", n);
	if (n > 0)
		printf(" string pool memory size %d bytes",
		       string_pool_get_len(pool));
	printf("\n");
#endif
	for (i = 0; i < n; i++) {
		char *s;

		s = string_pool_get_string(pool, n, i);
		printf("%s", s);
		if (i + 1 < n)
			printf(" ");
	}
}

void
string_pool_insert(string_pool_t *pool, char *s)
{
	unsigned char *buf;
	int blen, len, n, plen;

	assert(pool != NULL);
	assert(*pool != NULL);
	assert(s != NULL);

	/* Length of the new string to be added to the enum */
	len = strlen(s);

	/* n is the number of entries in the current string pool */
	n = string_pool_get_entries(*pool);

	/* plen is the length of the current string pool memory */
	plen = 0;
	if (n > 0)
		plen = string_pool_get_len(*pool);

#if _DEBUG
	printf("\nentries %d string pool memory length %d bytes\n", n, plen);
	bufdump((char *) ((*pool) + 3 + n * 2), plen);
#endif
	/* blen is the overall size of new string pool */
	blen = 3 + ((n + 1) * 2) + plen + len + 1;
#if _DEBUG
	printf("new string pool size %d bytes\n", blen);
#endif
	buf = malloc(blen);
	assert(buf != NULL);
	memset(buf, 0, blen);

	/* Setup number of entries in new string pool */
	string_pool_set_entries(buf, n + 1);

	/* Setup string pool memory length in new string pool */
	string_pool_set_len(buf, (unsigned short) (plen + len + 1));

	/* Copy array of indexes from old to new pool */
	memcpy(buf + 3, (*pool) + 3, n * 2);

	/* Setup index pointer for new string as last index in new pool */
	*((unsigned short *) (buf + 3 + n * 2)) = plen;

	/* Copy string pool memory from old to new pool */
	memcpy(buf + 3 + (n + 1) * 2, (*pool) + 3 + n * 2, plen);

	/* Copy new string to end of string pool memory */
	memcpy(buf + 3 + (n + 1) * 2 + plen, s, len);
#if _DEBUG
	bufdump((char *) buf, blen);
#endif
	free(*pool);
	*pool = (string_pool_t) buf;
}

char *
string_pool_find_by_idx(string_pool_t pool, int idx)
{
	int n;

	assert(pool != NULL);

	/* n is the number of entries in the current string pool */
	n = string_pool_get_entries(pool);

	if (idx >= n)
		return NULL;

	return string_pool_get_string(pool, n, idx);
}

int
string_pool_find_idx_by_name(string_pool_t pool, char *s)
{
	char *s1;
	int i, n;

	assert(pool != NULL);

	/* n is the number of entries in the current string pool */
	n = string_pool_get_entries(pool);

	for (i = 0; i < n; i++) {
		s1 = string_pool_get_string(pool, n, i);
		if (strcasecmp(s1, s) == 0)
			return i;
	}
	return (-1);
}
