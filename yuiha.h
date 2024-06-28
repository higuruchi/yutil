#include <stdbool.h>

#define O_VERSION       (1 << 22)
#define O_PARENT				(1 << 23) 
#define YUIHA_SUPER_MAGIC 0xEF54

enum yutil_command {
	VC,
	CAT,
	OVERWRITE,
  TEST,
};

enum io_type {
  READ,
  WRITE,
};

struct yutil_opt {
	enum yutil_command com;
	int path_len;
	char *path;
	int arg_len;
	char *arg;
  enum io_type io;
  int ind;
	bool parent_flg;
	bool mmap_flg;
  bool create_flg;
};

