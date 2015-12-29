#ifndef __SEED_BUF_H
#define __SEED_BUF_H

#define EmptyString ""
#define TAB "\t"
#define LF "\n"

#define BUF_MAX_PREALLOC (1<<20)

#include <stdio.h>
#include <stdarg.h>

#include "attribute.h"

typedef struct buf {
	size_t len;
	size_t free;
	char data[];
} buf;

typedef struct buf* Buf;

buf* bufNew(const char *init);
void bufRelease(Buf buf);
buf* bufCat(Buf buffer, const char *add);
buf* bufCatf(Buf buffer, const char *fmt, ...)
         __Attribute__((format(printf, 2, 3)));
buf* bufVcatf(Buf buffer, const char *fmt, va_list ap);

#endif
