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

#define OPTSTRING "p:a:o"
#define BUF_SIZE	4096

static struct option long_options[] = {
	{"path", required_argument, NULL, 'p'},
	{"arg", required_argument, NULL, 'a'},
	{"parent", no_argument, NULL, 'o'},
	{NULL, 0, NULL, 0},
};


int create_version(struct yutil_opt *yo, int mode)
{
	int fd = open(yo->path, mode | O_VERSION);
	if (fd < 0) {
		printf("failed to open file %s\n", yo->path);
		return -1;
	}

	printf("create version\n");
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

int overwrite(struct yutil_opt *yo)
{
	int ret, fd;
	if (yo->parent_flg)
		fd = open(yo->path, O_WRONLY | O_PARENT);
	else
		fd = open(yo->path, O_WRONLY);
	if (fd < 0) {
		printf("failed to open %s\n", yo->path);
		return -1;
	}

	ret = write(fd, yo->arg, yo->arg_len);
	if (ret < 0)
		printf("failed to write\n");
 
	return 0;
}

int main(int argc, char *argv[])
{
 	int c, option_index, ret, command_len;
	struct yutil_opt yo = {
			.parent_flg = false,
	};

	command_len = strlen(argv[1]);
	if (command_len == 2 && !strncmp(argv[1], "vc", 2)) {
		yo.com = VC;
	} else if (command_len == 2 && !strncmp(argv[1], "ow", 2)) {
		yo.com = OVERWRITE;
	} else if (command_len == 3 && !strncmp(argv[1], "cat", 3)) {
		yo.com = CAT;
	} else {}

	while (true) {
		if ((c = getopt_long(argc - 1, &argv[1], OPTSTRING, long_options,
                         &option_index)) == -1) {
			break;
		}

		switch (c) {
			case 'p':
				yo.path_len = strlen(optarg);
				yo.path = (char *)malloc(sizeof(char) * yo.path_len);
				strncpy(yo.path, optarg, yo.path_len);
				break;
			case 'a':
				yo.arg_len = strlen(optarg);
				yo.arg = (char *)malloc(sizeof(char) * yo.arg_len);
				strncpy(yo.arg, optarg, yo.path_len);
				break;
			case 'o':
				yo.parent_flg = true;
				break;
		}
	}

	switch (yo.com) {
	case VC:
		create_version(&yo, O_RDONLY);
		break;
	case CAT:
		cat_file(&yo);
		break;
	case OVERWRITE:
		overwrite(&yo);
		break;
	default:
		printf("command not found");
	}
	return 0;
}
