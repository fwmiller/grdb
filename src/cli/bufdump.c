#include <ctype.h>
#include <stdio.h>
#include <sys/types.h>

#define LEN	8

void
bufdump(char *buf, int size)
{
	u_char *line;
	int i, j, lines;

	lines = (size + LEN - 1) / LEN;
	for (i = 0; i < lines; i++) {
		line = (u_char *) buf + i * LEN;
		printf("%08llx  ", (unsigned long long) (buf + i * LEN));
		for (j = 0; j < LEN; j++)
			printf("%02x ", line[j]);
		printf(" ");
		for (j = 0; j < LEN; j++)
			if (isprint(line[j]))
				printf("%c", (char) line[j]);
			else
				printf(".");
		printf("\n");
	}
}
