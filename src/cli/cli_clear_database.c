#include <dirent.h>
#include "cli.h"

void
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
			char s[BUFSIZE];
			int ret;

			memset(s, 0, BUFSIZE);
			sprintf(s, "/bin/rm -fr %s/%s", grdbdir, de->d_name);
			ret = system(s);
			if (ret < 0)
				printf("clear database directory failed\n");
		}
	}
	closedir(dirfd);

	gno = (-1);
	cno = (-1);
}
