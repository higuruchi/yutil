#include <stdbool.h>

#define O_VERSION       (1 << 22)
#define O_PARENT				(1 << 23) 
#define YUIHA_SUPER_MAGIC 0xEF54

enum yutil_command {
	VC,
	CAT,
	OVERWRITE,
};

struct yutil_opt {
	enum yutil_command com;
	int path_len;
	char *path;
	int arg_len;
	char *arg;
	bool parent_flg;
	bool mmap_flg;
};

