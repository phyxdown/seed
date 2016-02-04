#ifndef __SEED_BUF_H
#define __SEED_BUF_H

//#define SEED_BUF_MAX_PREALLOC (1<<20)

#include <stdio.h>
#include <stdarg.h>

#include "attribute.h"

typedef struct seed_buf {
	size_t len;
	size_t free;
	char data[];
} seed_buf;

seed_buf* seed_buf_create();
seed_buf* seed_buf_create_with(const char *init);

void seed_buf_release(seed_buf* buf);

seed_buf* seed_buf_cat(seed_buf* buf, const char *add);

seed_buf* seed_buf_catf(seed_buf* buf, const char *fmt, ...)
	__seed_attribute__((format(printf, 2, 3)));

seed_buf* seed_buf_vcatf(seed_buf* buf, const char *fmt, va_list ap);

#endif
