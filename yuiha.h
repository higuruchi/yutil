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
#define YUIHA_IOC_STAT_VERSION	_IOR('f', 11, char *)

enum yutil_command {
	VC,
	CAT,
	OVERWRITE,
	TEST,
	DENT,
	DEL,
	VLN,
	STAT,
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

// struct yuiha_stat {
// 	unsigned short st_dev;
// 	unsigned short st_ino;
// 	unsigned short st_mode;
// 	unsigned short st_nlink;
// 	unsigned short st_uid;
// 	unsigned short st_gid;
// 	unsigned short st_rdev;
// 	unsigned long  st_size;
// 	unsigned long  st_atime;
// 	unsigned long  st_mtime;
// 	unsigned long  st_ctime;
// 	unsigned long  st_vtime;
// };

struct yuiha_stat {
 	unsigned short yst_dev;
 	unsigned short yst_ino;
 	unsigned short yst_mode;
 	unsigned short yst_nlink;
 	unsigned short yst_uid;
 	unsigned short yst_gid;
 	unsigned short yst_rdev;
	unsigned long  yst_size;
	unsigned long  yst_atime;
 	unsigned long  yst_mtime;
 	unsigned long  yst_ctime;
 	unsigned long  yst_vtime;
};

struct linux_dirent {
	unsigned long		d_ino;
	unsigned long		d_off;
	short						d_reclen;
	char						d_name[];
};
