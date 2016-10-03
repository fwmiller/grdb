#include <stdio.h>
#include <stdlib.h>
#include "string_pool.h"

void cli();

int
main(int argc, char**argv)
{
	string_pool_t pool = NULL;

	string_pool_init(&pool);
	string_pool_print(pool);

	printf("\ninsert \"this is a test\"\n");
	string_pool_insert(&pool, "this is a test");
	string_pool_print(pool);

	printf("\ninsert \"Another test\"\n");
	string_pool_insert(&pool, "Another test");
	string_pool_print(pool);

	printf("\ninsert \"Yet another\"\n");
	string_pool_insert(&pool, "Yet another");
	string_pool_print(pool);

	cli();
	exit(0);
}
