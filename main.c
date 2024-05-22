#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "yuiha.h"

#define OPTSTRING "s:c:p"
#define BUF_SIZE	4096

static struct option long_options[] = {
	{"snapshot", required_argument, NULL, 's'},
	{"cat", required_argument, NULL, 'c'},
	{"parent", no_argument, NULL, 'p'},
	{NULL, 0, NULL, 0},
};


int create_snapshot(struct yutil_opt *yo, int mode)
{
	int fd = open(yo->path, mode | O_VERSION);
	if (fd < 0) {
		printf("failed to open file %s\n", yo->path);
		return -1;
	}

	printf("create snapshot\n");
	return 0;
}

int cat_file(struct yutil_opt *yo)
{
	int ret, fd;
	char *buf = (char *)malloc(BUF_SIZE);
	if (!buf) {
		printf("memory allocation error\n");
		return -1;
	}

	if (yo->parent_flg)
		fd = open(yo->path, O_RDONLY | O_PARENT);
	else
		fd = open(yo->path, O_RDONLY);
	if (fd < 0) {
		printf("failed to open %s\n", yo->path);
		return -1;
	}

	do {
		ret = read(fd, buf, BUF_SIZE);
		write(1, buf, ret);
	}	while (0 < ret);

	return 0;
}

int main(int argc, char *argv[])
{
 	int c, option_index, ret;
	struct yutil_opt yo = {
			.parent_flg = false,
	};

	while (true) {
		if ((c = getopt_long(argc, argv, OPTSTRING, long_options,
                         &option_index)) == -1) {
			break;
		}

		switch (c) {
			case 's':
				yo.com = SNAPSHOT;
				yo.path_len = strlen(optarg);
				yo.path = (char *)malloc(sizeof(char) * yo.path_len);
				strncpy(yo.path, optarg, yo.path_len);
				break;
			case 'c':
				yo.com = CAT;
				yo.path_len = strlen(optarg);
				yo.path = (char *)malloc(sizeof(char) * yo.path_len);
				strncpy(yo.path, optarg, yo.path_len);
				break;
			case 'p':
				yo.parent_flg = true;
				break;
		}
	}

	switch (yo.com) {
	case SNAPSHOT:
		create_snapshot(&yo, O_RDONLY);
		break;
	case CAT:
		cat_file(&yo);
		break;
	default:
		printf("command not found");
	}
	return 0;
}
