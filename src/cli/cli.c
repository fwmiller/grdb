#include <assert.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "cli.h"
#include "graph.h"

/* Home directory for user */
const char *homedir;
char grdbdir[BUFSIZE];

/* Current graph and component numbers */
int gno = (-1), cno = (-1);

char *readline(char *prompt);

static int tty = 0;

static void
cli_about()
{
	printf("Graph Database\n");
	printf("(C) Frank W. Miller\n");
}

static void
cli_clear_database()
{
	DIR *dirfd;
	struct dirent *de;

	if ((dirfd = opendir(grdbdir)) == NULL)
		return;

	for (;;) {
		de = readdir(dirfd);
		if (de == NULL)
			break;

		if (strcmp(de->d_name, ".") != 0 &&
		    strcmp(de->d_name, "..") != 0) {
			if (fork() == 0) {
				char s[BUFSIZE];

				memset(s, 0, BUFSIZE);
				sprintf(s, "%s/%s", grdbdir, de->d_name);
#if _DEBUG
				printf("cli_clear_database: ");
				printf("remove directory: %s\n", s);
#endif
				execl("/bin/rm", "/bin/rm", "-fr", s, NULL);
			}
		}
	}
	closedir(dirfd);
}

static int
cli_find_low_gno()
{
	/* XXX This needs to be fancier */
	return 0;
}

static int
cli_find_low_cno(int gno)
{
	/* XXX This needs to be fancier */
	return 0;
}

static void
cli_help()
{
	return;
}

void
cli()
{
	char *cmdline = NULL;
	char cmd[BUFSIZE], prompt[BUFSIZE];
	int pos;
	struct stat sb;

	tty = isatty(STDIN_FILENO);
	if (tty)
		cli_about();

	/* Setup to load databases */
	homedir = getenv("HOME");
	memset(grdbdir, 0, BUFSIZE);
	strcpy(grdbdir, homedir);
	strcat(grdbdir, "/.grdb");
#if _DEBUG
	printf("grdb directory: %s\n", grdbdir);
#endif
	/* Check for existence of grdb directory */
	if (stat(grdbdir, &sb) != 0 || !S_ISDIR(sb.st_mode)) {
#if _DEBUG
		printf("creating grdb directory\n");
#endif
		mkdir(grdbdir, 0755);
	}

	/* Initalize gno and cno */
	gno = cli_find_low_gno();
	cno = cli_find_low_cno(gno);

	/* Main command line loop */
	for (;;) {
		if (cmdline != NULL) {
			free(cmdline);
			cmdline = NULL;
		}
		// cmdline = readline(PROMPT);
		memset(prompt, 0, BUFSIZE);
		sprintf(prompt, "grdb> ");

		if (tty)
			cmdline = readline(prompt);
		else
			cmdline = readline("");

		if (cmdline == NULL)
			continue;

		if (strlen(cmdline) == 0)
			continue;

		if (!tty)
			printf("%s\n", cmdline);

		if (strcmp(cmdline, "?") == 0) {
			cli_help();
			continue;
		}
		if (strcmp(cmdline, "quit") == 0 ||
		    strcmp(cmdline, "q") == 0)
			break;

		memset(cmd, 0, BUFSIZE);
		pos = 0;
		nextarg(cmdline, &pos, " ", cmd);

		if (strcmp(cmd, "about") == 0 || strcmp(cmd, "a") == 0) {
			cli_about();
			continue;

		} else if (strcmp(cmd, "clear") == 0 ||
			   strcmp(cmd, "c") == 0) {
			printf("clear database\n");
			cli_clear_database();
			continue;

		} else if (strcmp(cmd, "help") == 0 ||
			   strcmp(cmd, "h") == 0) {
			cli_help();
			continue;

		} else if (strcmp(cmd, "enum") == 0 ||
			   strcmp(cmd, "e") == 0) {
			cli_enum(cmdline, &pos);
			continue;

		} else if (strcmp(cmd, "graph") == 0 ||
			   strcmp(cmd, "g") == 0) {
			cli_graph(cmdline, &pos);
			continue;
		}
	}
}
