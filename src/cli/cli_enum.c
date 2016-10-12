#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "enum.h"

int
cli_enum_syntax_check(char *s)
{
	int i, len;
	char ch;

	assert(s != NULL);

	/* XXX This can be made more efficient */
	for (len = strlen(s), i = 0; i < len; i++) {
		ch = s[i];
		if (i == 0) {
			if (!isalnum(ch))
				return 0;

		} else if (!isalnum(ch) && ch != '-' && ch != '_')
			return 0;
	}
	return 1;
}

void
cli_enum(char *cmdline, int *pos)
{
	char s[BUFSIZE];
	int result;

	assert(cmdline != NULL);
	assert(pos != NULL);

	memset(s, 0, BUFSIZE);
	nextarg(cmdline, pos, " ", s);

	if (strlen(s) == 0) {
		/* Dump list of enums for all graphs */

		return;
	}
	/* Try to create a new enum */

	/* Check syntax of enum name */
	result = cli_enum_syntax_check(s);
	if (!result) {
		printf("Enum name illegal syntax\n");
		return;
	}
	/* Check whether enum name is a duplicate */

	/* Create new enum */

	/* Add elements to enum */
}
