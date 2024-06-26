#ifndef _ERR_H_
#define _ERR_H_

void
err_msg(const char *file,
	const char *function,
	int line,
	const char *type,
	const char *fmt, ...);

#define ERROR(fmt, ...) \
		err_msg(__FILE__, __FUNCTION__, __LINE__, "error", fmt, ##__VA_ARGS__)
#define WARNNING(fmt, ...) \
		err_msg(__FILE__, __FUNCTION__, __LINE__, "warnning", fmt, ##__VA_ARGS__)

#endif // _ERR_H_
