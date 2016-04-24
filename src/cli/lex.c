#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"

char *
readline(char *prompt)
{
	char *buf;
	int c, pos;

	buf = malloc(BUFSIZE);
	if (buf == NULL)
		return NULL;

	memset(buf, 0, BUFSIZE);

	printf("%s", prompt);
	for (pos = 0; pos < BUFSIZE - 1; pos++) {
		c = getchar();
		if (c == '\n')
			break;
		buf[pos] = c;
	}
	return buf;
}

static int
issep(char *sep, char ch)
{
	int i, len;

	if (sep == NULL)
		return 0;

	for (len = strlen(sep), i = 0; i < len; i++)
		if (ch == sep[i])
			return 1;
	return 0;
}

void
nextarg(char *ln, int *pos, char *sep, char *arg)
{
	char *s;
	char ch;

	if (ln == NULL || pos == NULL || arg == NULL)
		return;

	s = arg;

	/* Skip whitespace */
	ch = ln[*pos];
	while (isspace(ch))
		ch = ln[++(*pos)];

	/* Fill in arg until a separator is reached */
	strcpy(s, "");
	while (ch != '\0' && !issep(sep, ch)) {
		*(s++) = ch;
		ch = ln[++(*pos)];
	};
	*s = '\0';
}
