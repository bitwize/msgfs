
all: msgfs_test

msgfs_test: msgfs_test.c msgfs.c
	gcc -g msgfs_test.c msgfs.c -o msgfs_test
