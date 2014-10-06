#include "msgfs.h"

int main(void)
{
	char buf[4096];
	int payload_fd, n;
	msgfs_init();
	int msg_id = msgfs_listen("/tmp/msgfs_test");
	while(1) {
		payload_fd = msgfs_get(msg_id);
		if(payload_fd < 0) {
			fprintf(stderr, "error opening message\n");
			exit(-1);
		}
		while((n=read(payload_fd, buf, 4095)) < 0)
		{
			if(errno != EAGAIN && errno != EINTR)
			{
				fprintf(stderr, "error reading message\n");
				exit(-1);
			}
		}
		buf[n] = '\0';
		printf("msgfs says: %s\n", buf);	
	}
}
