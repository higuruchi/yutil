#include <stdbool.h>

#define O_VERSION       020000000
#define O_PARENT				040000000 
#define O_VSEARCH       0200000000
#define YUIHA_SUPER_MAGIC 0xEF54

#define DT_PARENT   020
#define DT_SIBLING  040
#define DT_CHILD    0100
#define DT_VROOT    0200
#define DT_VCURRENT 0400

enum yutil_command {
	VC,
	CAT,
	OVERWRITE,
  TEST,
  DENT,
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
  int ino;
	bool parent_flg;
	bool mmap_flg;
  bool create_flg;
};

struct linux_dirent {
  long           d_ino;
  off_t          d_off;
  unsigned short d_reclen;
  char           d_name[];
};
