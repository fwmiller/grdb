#include <assert.h>
#if _DEBUG
#include <stdio.h>
#endif
#include <stdlib.h>
#include <string.h>
#include "string_pool.h"

void
string_pool_init(string_pool_t *pool)
{
	assert(pool != NULL);

	if (*pool != NULL)
		free(*pool);

	*pool = malloc(1);
	assert(*pool != NULL);
	(*pool)[0] = 0;
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
	n = (int) pool[0];
	
	printf("entries %d", n);
	if (n > 0)
		printf(" string pool memory size %d bytes",
			*((unsigned short *) (pool + 1)));
	printf("\n");

	for (i = 0; i < n; i++) {
		unsigned short idx;
		unsigned char *pos;

		idx = *((unsigned short *) (pool + 3 + i * 2));
		pos = pool + 3 + n * 2;
		printf("[%s]\n", pos + idx);
	}
}

void
string_pool_insert(string_pool_t *pool, char *s)
{
	char *buf;
	int blen, len, n, plen;

	assert(pool != NULL);
	assert(*pool != NULL);
	assert(s != NULL);

	/* Length of the new string to be added to the enum */
	len = strlen(s);

	/* n is the number of entries in the current string pool */
	n = (*pool)[0];

	/* plen is the length of the current string pool memory */
	plen = (n > 0 ? *((unsigned short *) ((*pool) + 1)) : 0);

#if _DEBUG
	printf("entries %d string pool memory length %d bytes\n", n, plen);
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
	*buf = (unsigned char) n + 1;

	/* Setup string pool memory length in new string pool */
	*((unsigned short *) (buf + 1)) = plen + len + 1;

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
