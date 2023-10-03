#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "yuiha.h"

#define OPTSTRING "s:"

static struct option long_options[] = {
	{"snapshot", required_argument, NULL, 's'},
	{NULL, 0, NULL, 0}
};

int main(int argc, char *argv[])
{
 	int c, option_index, ret;

	while (true) {
		if ((c = getopt_long(argc, argv, OPTSTRING, long_options,
                         &option_index)) == -1) {
			break;
		}

		switch (c) {
		case 's':
			ret = open(optarg, O_RDWR | O_VERSION);
			if (ret < 0)
				printf("failed to open file %s\n", optarg);
			else
				puts("create snapshot");
			break;
		}
	}

	return 0;
}
