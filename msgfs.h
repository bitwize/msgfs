#ifndef __MSGFS_H
#define __MSGFS_H

#include <linux/limits.h>
#include <sys/inotify.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int msgfs_init();
int msgfs_listen(char *path);
int msgfs_get(int wd);


#endif
