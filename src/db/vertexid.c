#include <stdlib.h>
#include <time.h>
#include "vertexid.h"

static int seeded = 0;

vertexid_t vertexid_rand()
{
	if (!seeded) {
		srand(time(NULL));
		seeded = 1;
	}
	return (vertexid_t) (rand() % 0x10000);
}
