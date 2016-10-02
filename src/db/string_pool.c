#include <assert.h>
#if _DEBUG
#include <stdio.h>
#endif
#include <stdlib.h>
#include <string.h>

/*****************************************************************************
******************************************************************************


A string pool is a list of strings and an array index to their locations
in a single allocated block of memory.

+---------+----------------------------------------+--------------------+
| entries | string pool   | array of indexes into  | string pool memory |
|         | memory length | string pool memory     |                    |
+---------+----------------------------------------+--------------------+
^byte idx ^               ^                        ^
|         |               |                        |
0         1               3                        entries * 2 + 3

The entries field contains the number of strings in the string pool.  This
field is a byte, which limits the number of entries to 256.

The string pool memory length field contains a 16-bit value that is
the length of the string pool memory buffer in bytes

The array of indexes contains the list of pointers to the strings in
the string pool memory buffer.  Each index is 16-bit value.  This
effectively limits the overall size of the string pool to ~64 KB.

The string pool memory buffer is a contiguous block of bytes that is used
to hold the concatenated strings.

When a entry is added or removed, the string pool is reallocated and the
proper data is copied from the old to the new version.


******************************************************************************
*****************************************************************************/


typedef unsigned char *string_pool_t;

#if _DEBUG
void bufdump(char *buf, int size);
#endif

void
string_pool_init(string_pool_t *pool)
{
	assert(pool != NULL);

	if (*pool != NULL)
		free(*pool);

	*pool = malloc(1);
	assert(*pool != NULL);
	*pool = 0;
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
	plen = *((unsigned short *) (pool + 1));

#if _DEBUG
	printf("entries %d string pool memory length %d bytes\n", n, plen);
	bufdump((char *) (pool + 3 + n * 2), plen);
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
	memcpy(buf + 3, pool + 3, n * 2);

	/* Setup index pointer for new string as last index in new pool */
	*((unsigned short *) (buf + 3 + n * 2)) = plen;

	/* Copy string pool memory from old to new pool */
	memcpy(buf + 3 + (n + 1) * 2, pool + 3 + n * 2, plen);

	/* Copy new string to end of string pool memory */
	memcpy(buf + 3 + (n + 1) * 2 + plen, s, len);

#if _DEBUG
	bufdump((char *) buf, blen);
#endif
	free(*pool);
	*pool = (string_pool_t) buf;
}
