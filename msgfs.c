#include "msgfs.h"

static int msgfs_notfd;
char msgfs_path[1024][PATH_MAX];
int
msgfs_init()
{
	msgfs_notfd = inotify_init1(0);
	if(msgfs_notfd < 0)
		return -1;
	return 0;
}

int
msgfs_listen(char *path)
{
	int i = inotify_add_watch(msgfs_notfd, path, IN_CREATE | IN_CLOSE_WRITE);
	if(i>=0) {
		strncpy(msgfs_path[i], path, PATH_MAX);
		msgfs_path[i][PATH_MAX - 1] = '\0';
	}
	return i;
}

char *
pathcat(char *dest, char *dir, char *file, size_t n)
{
	size_t dir_len = strlen(dir);
	size_t file_len = strlen(file);
	if(n < dir_len + file_len + 2) {
		fprintf(stderr, "pathname too long!\n");
		return NULL;
	}
	strncpy(dest, dir, n);
	if(n > 0) {
		dest[n-1] = '\0';
	}
	if(dest[dir_len - 1] != '/') {
		dest[dir_len] = '/';
		dest[dir_len + 1] = '\0';
	}
	strncat(dest, file, n - dir_len - 2);
	return dest;
}

int msgfs_get(int wd)
{
	char eventbuf[sizeof(struct inotify_event) + NAME_MAX + 1];
	char pathbuf[PATH_MAX];
	struct inotify_event *event = (struct inotify_event *)eventbuf;
	do {
		int n = read(msgfs_notfd, (void *)eventbuf, sizeof(eventbuf));
		if(n <= 0)
		{
			return -1;
		}
		if((event->wd == wd) &&
		   (event->mask & IN_CLOSE_WRITE) &&
		   (event->len > 0)) {
			char *pb = pathcat(pathbuf, msgfs_path[wd], event->name, PATH_MAX);
			if(pb != NULL) {
				return(open(pathbuf, O_RDONLY));
			}
			else {
				return -1;
			}
		}
	} while(1);
}
