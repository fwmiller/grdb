#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "enum.h"
#include "graph.h"

extern graph_t current, graphs;

int graphs_get_current_index();

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
cli_enum_print_current()
{
	enum_t e;

	if (current != NULL && current->el != NULL) {
#if _DEBUG
		printf("graph %d\n", graphs_get_current_index());
#endif
		for (e = current->el; e != NULL; e = e->next) {
			printf("%s (", e->name);
			string_pool_print(e->pool);
			printf(")\n");
		}
	}
}

void
cli_enum(char *cmdline, int *pos)
{
	enum_t e = NULL;
	char s[BUFSIZE];
	int result;

	assert(cmdline != NULL);
	assert(pos != NULL);

	memset(s, 0, BUFSIZE);
	nextarg(cmdline, pos, " ", s);

	if (strlen(s) == 0) {
		cli_enum_print_current();
		return;
	}
	/* Try to create a new enum */

	/* Check syntax of enum name */
	result = cli_enum_syntax_check(s);
	if (!result) {
		printf("Enum name illegal syntax\n");
		return;
	}
#if _DEBUG
	printf("enum %s\n", s);
#endif
	/* Check whether enum name is a duplicate */
	if (current != NULL)
		for (e = current->el; e != NULL; e = e->next)
			if (strcmp(s, enum_get_name_ptr(e)) == 0) {
				printf("enum %s already exists\n", s);
				return;
			}

	/* Create new enum */
	enum_init(&e);

	/* Copy name */
	strncpy(e->name, s, ENUM_NAME_LEN - 1);

	/* Add elements to enum */
	for (;;) {
		memset(s, 0, BUFSIZE);
		nextarg(cmdline, pos, " ", s);

		if (strlen(s) == 0)
			break;

		result = cli_enum_syntax_check(s);
		if (!result) {
			printf("Enum element %s illegal syntax\n", s);

			/* XXX Free enum memory */

			return;
		}
		enum_insert(&e, s);
	}
	/* XXX Check whether enum has any elements in it */

	/* Insert enum in list of enums for current graph */
	enum_list_insert(&(current->el), e);
}
