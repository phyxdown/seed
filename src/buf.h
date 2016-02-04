#ifndef __SEED_BUF_H
#define __SEED_BUF_H

#include <stdio.h>
#include <stdarg.h>

#include "attribute.h"

typedef struct seed_buf {
	size_t len;
	size_t free;
	char data[];
} seed_buf;

//To ensure that all seed_buf functions does not panic, 
//the using program MUST check if it is a NULL-seed_buf created, 
//and make sure that NEVER a NULL-seed_buf is used.
seed_buf* seed_buf_create();
seed_buf* seed_buf_create_with_string(const char *init);

const char* seed_buf_data(seed_buf* buf);

void seed_buf_release(seed_buf* buf);

seed_buf* seed_buf_cat(seed_buf* buf, const char *add);

seed_buf* seed_buf_catf(seed_buf* buf, const char *fmt, ...)
	__seed_attribute__((format(printf, 2, 3)));

seed_buf* seed_buf_vcatf(seed_buf* buf, const char *fmt, va_list ap);

#endif
