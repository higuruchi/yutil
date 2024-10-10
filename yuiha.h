#include <stdbool.h>
#include <sys/ioctl.h>

#define O_VERSION       020000000
#define O_PARENT				040000000 
#define O_VSEARCH       0200000000
#define YUIHA_SUPER_MAGIC 0xEF54

#define DT_PARENT   020
#define DT_CHILD    040
#define DT_VROOT    0100

#define YUIHA_IOC_DEL_VERSION		_IO('f', 9)

enum yutil_command {
	VC,
	CAT,
	OVERWRITE,
  TEST,
  DENT,
	DEL,
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
	unsigned long		d_ino;
	unsigned long 	d_off;
	short						d_reclen;
	char						d_name[];
};
