#include <stdbool.h>
#include <sys/ioctl.h>

#define O_VERSION				020000000
#define O_PARENT				040000000 
#define O_VSEARCH				0200000000
#define YUIHA_SUPER_MAGIC 0xEF54

#define DT_PARENT		020
#define DT_CHILD		040
#define DT_VROOT		0100

#define YUIHA_IOC_DEL_VERSION		_IOWR('f', 9, unsigned long)
#define YUIHA_IOC_LINK_VERSION	_IOW('f', 10, char *)
#define YUIHA_IOC_GET_ROOT			_IOR('f', 11, unsigned int)

enum yutil_command {
	VC,
	CAT,
	OVERWRITE,
	TEST,
	DENT,
	DEL,
	VLN,
	STAT,
	RV,
};

enum io_type {
	READ,
	WRITE,
};

struct yutil_opt {
	enum yutil_command com;
	int path_len;
	char *path;
	int target_len;
	char *target;
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
	unsigned long		d_off;
	short						d_reclen;
	char						d_name[];
};
