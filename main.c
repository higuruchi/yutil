#define _LARGEFILE64_SOURCE
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/syscall.h>

#include "err.h"
#include "yuiha.h"

#define OPTSTRING "p:a:i:n:d:omc"
#define BUF_SIZE  4096

static struct option long_options[] = {
  {"path", required_argument, NULL, 'p'},
  {"arg", required_argument, NULL, 'a'},
  {"io-type", required_argument, NULL, 'i'},
  {"ind", required_argument, NULL, 'n'},
  {"inode", required_argument, NULL, 'd'},
  {"parent", no_argument, NULL, 'o'},
  {"mmap", no_argument, NULL, 'm'},
  {"create", no_argument, NULL, 'c'},
  {NULL, 0, NULL, 0},
};


int create_version(struct yutil_opt *yo, int mode)
{
  int fd = open(yo->path, mode | O_VERSION);
  if (fd < 0) {
    ERROR("Failed to open file %s\n", yo->path);
    return -1;
  }

  printf("create version\n");
  return 0;
}

int cat_file(struct yutil_opt *yo)
{
  int ret, fd, flags = O_RDONLY;
  char *buf = (char *)malloc(BUF_SIZE);
  if (!buf) {
    ERROR("Memory allocation error\n");
    return -1;
  }

  if (yo->parent_flg)
    flags |= O_PARENT;
  if (yo->ino)
    flags |= O_CREAT | O_VSEARCH;

  fd = open(yo->path, flags, yo->ino);
  if (fd < 0) {
    ERROR("Failed to open file %s\n", yo->path);
    return -1;
  }

  do {
    ret = read(fd, buf, BUF_SIZE);
    write(1, buf, ret);
  } while (0 < ret);

  return 0;
}

int overwrite_using_mmap(struct yutil_opt *yo, int fd)
{
  char *map;
  long page_size, map_size;

  page_size = getpagesize();
  map_size = (yo->arg_len / page_size + 1) * page_size;

  map = (char *)mmap(NULL, map_size, PROT_WRITE, MAP_SHARED, fd, 0);
  if (map == MAP_FAILED) {
    ERROR("Mmap failed\n");
    return -1;
  }
  strncpy(map, yo->arg, yo->arg_len);

  munmap(map, map_size);
  close(fd);

  return 0;
}

int overwrite(struct yutil_opt *yo)
{
  int ret, fd, flags = O_WRONLY;
  mode_t mode;

  if (yo->create_flg && yo->ino) {
    ERROR("Invalid option\n");
    return -1;
  }

  if (yo->parent_flg)
    flags |= O_PARENT;
  if (yo->create_flg) {
    mode = 0644;
    flags |= O_CREAT;
  }
  if (yo->ino) {
    mode = yo->ino;
    flags |= O_CREAT | O_VSEARCH;
  }

  fd = open(yo->path, flags, mode);
  if (fd < 0) {
    ERROR("Failed to open file %s\n", yo->path);
    return -1;
  }

  if (yo->mmap_flg) {
    ret = overwrite_using_mmap(yo, fd);
  } else {
    ret = write(fd, yo->arg, yo->arg_len);
  }

  if (ret < 0)
    ERROR("Failed to write\n");
  return 0;
}

int file_io_test(struct yutil_opt *yo)
{
  int fd, ret, flags = O_CREAT;
  mode_t mode = 0644;
  char *buf = (char *)malloc(4096);
  off64_t offset = 0;
  
  memset(buf, 'a', 4096);

  switch (yo->io) {
    case WRITE:
      flags |= O_RDWR;
    case READ:
      flags |= O_RDONLY;
  }

  switch (yo->ind) {
    case 0:
      offset = 0;
      break;
    case 1:
      offset = 12*4096;
      break;
    case 2:
      offset = 12*4096+1024*4096; 
      break;
    case 3:
      offset = 1024*1024;
      offset *= 4096;
      offset += 12*4096;
      break;
  }

  fd = open(yo->path, flags, mode);
  if (fd < 0) {
    ERROR("Failed to open file %s\n", yo->path);
    return -1;
  }
  
  lseek64(fd, offset, SEEK_SET);
  ret = write(fd, buf, 4096);
  if (ret < 0) {
    ERROR("Failed to write\n");
    return -1;
  }

  return 0;
}

int list_file_versions(struct yutil_opt *yo) {
  char buf[1024];
  int buf_off = 0;
  struct linux_dirent *d;
  int fd, nread, flags = O_RDONLY;
  unsigned short *flag_p;
  mode_t mode;
  
  if (yo->parent_flg)
    flags |= O_PARENT;
  if (yo->ino) {
    mode = yo->ino;
    flags |= O_VSEARCH | O_CREAT;
  }

  fd = open(yo->path, flags, mode);
  if (fd < 0) {
    ERROR("Failed to open file %s\n", yo->path);
    return -1;
  }

  nread = syscall(SYS_getdents, fd, buf, 1024);
  if (nread < 0) {
    ERROR("Failed to get version entry\n");
    return -1;
  }
  
  while (buf_off < nread) {
    d = (struct linux_dirent *) (buf + buf_off);
    flag_p = (unsigned short *)d->d_name;
    
    if (*flag_p & DT_PARENT)
      printf("ino=%lu is a parent.", d->d_ino);
    if (*flag_p & DT_SIBLING)
      printf("ino=%lu is a sibling.", d->d_ino);
    if (*flag_p & DT_CHILD)
      printf("ino=%lu is a child.", d->d_ino);
    if (*flag_p & DT_VROOT)
      printf("root.");
    if (*flag_p & DT_VCURRENT)
      printf("current.");
    puts("");

    buf_off += d->d_reclen;
  }
  return 0;
}

int main(int argc, char *argv[])
{
  int c, option_index, ret, command_len;
  struct yutil_opt yo = {
      .ino = 0,
      .parent_flg = false,
      .mmap_flg = false,
      .create_flg = false,
  };

  command_len = strlen(argv[1]);
  if (command_len == 2 && !strncmp(argv[1], "vc", 2)) {
    yo.com = VC;
  } else if (command_len == 2 && !strncmp(argv[1], "ow", 2)) {
    yo.com = OVERWRITE;
  } else if (command_len == 3 && !strncmp(argv[1], "cat", 3)) {
    yo.com = CAT;
  } else if (command_len == 4 && !strncmp(argv[1], "test", 4)) {
    yo.com = TEST;
  } else if (command_len == 4 && !strncmp(argv[1], "dent", 2)) {
    yo.com = DENT;
  }

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
        strncpy(yo.arg, optarg, yo.arg_len);
        break;
      case 'i':
        if (!strncmp(optarg, "read", 4)) {
          yo.io = READ;
        } else if (!strncmp(optarg, "write", 4)) {
          yo.io = WRITE;
        }
        break;
      case 'n':
        yo.ind = atoi(optarg);
        break;
      case 'd':
        yo.ino = atoi(optarg);
        break;
      case 'o':
        yo.parent_flg = true;
        break;
      case 'm':
        yo.mmap_flg = true;
        break;
      case 'c':
        yo.create_flg = true;
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
  case TEST:
    file_io_test(&yo);
    break;
  case DENT:
    list_file_versions(&yo);
    break;
  default:
    ERROR("Command not found\n");
    return -1;
  }
  return 0;
}
