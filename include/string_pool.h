#ifndef __STRING_POOL_H
#define __STRING_POOL_H


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

void string_pool_init(string_pool_t *pool);
void string_pool_print(string_pool_t pool);
void string_pool_insert(string_pool_t *pool, char *s);
char *string_pool_find_by_idx(string_pool_t pool, int idx);
int string_pool_find_idx_by_name(string_pool_t pool, char *s);


#endif
