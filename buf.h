#ifndef __BUF_H__
#define __BUF_H__

#define EmptyString ""
#define TAB "\t"
#define LF "\n"

#define BUF_MAX_PREALLOC (1<<20)

#include <stdio.h>
#include <stdarg.h>

#include "defs.h"

struct buf {
	size_t len;
	size_t free;
	char data[];
};

typedef struct buf* Buf;

Buf bufnew(const char *init);

Buf bufcat(Buf buffer, const char *add);

Buf bufcatf(Buf buffer, const char *fmt, ...)
	__Attribute__((format(printf, 2, 3)));

Buf bufvcatf(Buf buffer, const char *fmt, va_list ap);

#endif /* __BUF_H__ */
