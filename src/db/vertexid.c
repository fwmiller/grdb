#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "vertexid.h"


#define START_CAPACITY		8	/* Neighbors */

static int seeded = 0;

vertexid_t vertexid_rand()
{
	if (!seeded) {
		srand(time(NULL));
		seeded = 1;
	}
	return (vertexid_t) (rand() % 0x10000);
}

void
vertexid_set_alloc(vertexid_set_t s)
{
	assert(s != NULL);

	s->capacity = START_CAPACITY;
	s->n = 0;
	s->arr = malloc(s->capacity * sizeof(vertexid_t));
}

void
vertexid_set_free(vertexid_set_t s)
{
	assert(s != NULL);

	s->capacity = 0;
	s->n = 0;
	if (s->arr != NULL) {
		free(s->arr);
		s->arr = NULL;
	}
}

void
vertexid_set_insert(vertexid_set_t s, vertexid_t id)
{
	assert(s != NULL);
	assert(s->n < s->capacity);

	s->arr[(s->n)++] = id;
	if (s->n == s->capacity) {
		vertexid_t *arr = NULL;

		/* Double the capacity of the array */
		s->capacity <<= 1;
		arr = (vertexid_t *) malloc(s->capacity * sizeof(vertexid_t));
		assert(arr != NULL);
		memset(arr, 0, s->capacity * sizeof(vertexid_t));
		memcpy(s->arr, arr, s->n * sizeof(vertexid_t));
		free(s->arr);
		s->arr = arr;
	}
}

void
vertexid_set_remove(vertexid_set_t s, vertexid_t id)
{
	int i;

	assert(s != NULL);

	/* Search array for id */
	for (i = 0; i < s->n; i++)
		if (s->arr[i] == id)
			break;

	if (i == s->n) {
		/* id not found */
		return;
	}
	if (i < s->n - 1) {
		/* Shift array contents to fill gap left by removed id */
		for (; i < s->n - 1; i++)
			s->arr[i] = s->arr[i + 1];
	}
	s->arr[s->n - 1] = 0;
	s->n--;
}

void
vertexid_set_print(vertexid_set_t s)
{
	int i;

	assert(s != NULL);

	printf("{");
	for (i = 0; i < s->n; i++) {
		printf("%04llx", s->arr[i]);
		if (i + 1 < s->n)
			printf(",");
	}
	printf("}");
}
