#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "cli.h"
#include "enum.h"
#include "graph.h"

int
cli_graph_enum_syntax_check(char *s)
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
cli_graph_enum_print_current()
{
	enum_list_t el = NULL;
	int fd;

	if (gno < 0 || cno < 0)
		return;

	fd = enum_file_open(grdbdir, gno, cno);
	if (fd < 0)
		return;

	enum_list_init(&el);
	el = enum_list_read(&el, fd);
	close(fd);

	if (el != NULL) {
		printf(">component %d.%d\n", gno, cno);
		enum_list_print(el);
	}
}

void
cli_graph_enum(char *cmdline, int *pos)
{
	enum_list_t el = NULL;
	enum_t e = NULL;
	char s[BUFSIZE];
	int fd;

	assert(cmdline != NULL);
	assert(pos != NULL);

	memset(s, 0, BUFSIZE);
	nextarg(cmdline, pos, " ", s);

	if (strlen(s) == 0) {
		cli_graph_enum_print_current();
		return;
	}
	/* Try to create a new enum */
	if (gno < 0 || cno < 0) {
		printf("Missing component\n");
		return;
	}
	/* Check syntax of enum name */
	if (!cli_graph_enum_syntax_check(s)) {
		printf("Enum name illegal syntax\n");
		return;
	}
#if _DEBUG
	printf("enum %s\n", s);
#endif
	fd = enum_file_open(grdbdir, gno, cno);
	if (fd < 0)
		return;

	enum_list_init(&el);
	enum_list_read(&el, fd);
#if _DEBUG
	enum_list_print(el);
#endif

	/* Check whether enum name is a duplicate */
	for (e = el; e != NULL; e = e->next)
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

		if (!cli_graph_enum_syntax_check(s)) {
			printf("Enum element %s illegal syntax\n", s);

			/* XXX Free enum memory */

			return;
		}
		enum_insert(&e, s);
	}
	/* XXX Check whether enum has any elements in it */

	/* Insert enum in list of enums for current component */
	enum_list_insert(&el, e);

	enum_list_write(el, fd);
	close(fd);
}
