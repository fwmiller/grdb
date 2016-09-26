#include <assert.h>
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


typedef char *string_pool_t;

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

	len = strlen(s);
	n = (*pool)[0];
	plen = *((unsigned short *) (pool + 1));
#if _DEBUG
	printf("entries %d string pool memory length %d bytes\n", n, plen);
#endif
	blen = 3 + ((n + 1) * 2) + plen + strlen(s) + 1;
#if _DEBUG
	printf("new string pool memory length %d bytes\n", blen);
#endif
	buf = malloc(blen);
	assert(buf != NULL);
}
