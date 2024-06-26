#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

void
err_msg(const char *file,
	const char *function,
	int line,
	const char *type,
	const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);

	fprintf(stderr, "%s %s %d %s ", file, function, line, type);
	fprintf(stderr, "%s(%d)", strerror(errno), errno);
	fputc('\n', stderr);
}
