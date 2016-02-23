#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "buf.h"

#define SEED_BUF_MAX_PREALLOC (1<<20)

seed_buf* seed_buf_create() {
	return seed_buf_create_with_string("");
}

seed_buf* seed_buf_create_with_string(const char *init) {
	seed_buf* buf;
	size_t initlen = strlen(init);
	if (NULL == (buf = malloc(sizeof(struct seed_buf) + initlen + 1)))
		return NULL;
	buf->len = 0;
	buf->free = initlen;
	buf->data[buf->len] = '\0';
	buf = seed_buf_cat(buf, init);
	return buf;
}

const char* seed_buf_data(seed_buf* buf) {
	return buf->data;
}

void seed_buf_release(seed_buf* buf) {
	if (NULL != buf) free(buf);
}

seed_buf* seed_buf_cat(seed_buf* bufin, const char *add) {
	seed_buf *buf = bufin;
	size_t addlen = strlen(add);
	if (addlen == 0) return buf;
	if (addlen > buf->free) {
		size_t newlen = buf->len + addlen;
		if (newlen < SEED_BUF_MAX_PREALLOC) newlen *= 2;
		else newlen += SEED_BUF_MAX_PREALLOC;
		seed_buf* nbuf = realloc(buf, sizeof(struct seed_buf)+ newlen + 1);
		if (nbuf == NULL) return buf;
		else buf = nbuf;
		buf->free = newlen - buf->len;
	}
	strcpy(&buf->data[buf->len], add);
	buf->len += addlen;
	buf->free -= addlen;
	buf->data[buf->len] = '\0';
	return buf;
}

seed_buf* seed_buf_vcatf(seed_buf* bufin, const char *fmt, va_list ap) {
	seed_buf* buf = bufin;
	va_list cpy;
	char staticbuf[1024], *buffer = staticbuf;
	size_t buflen = strlen(fmt)*2;
	if (buflen > sizeof(staticbuf)) {
		buffer = malloc(buflen);
		if (buffer == NULL) return buf;
	} else {
		buflen = sizeof(staticbuf);
	}
	while(1) {
		buffer[buflen-2] = '\0';
		va_copy(cpy, ap);
		vsnprintf(buffer, buflen, fmt, cpy);
		va_end(cpy);
		if (buffer[buflen-2] != '\0') {
			if (buffer != staticbuf) free(buffer);
			buflen *= 2;
			buffer = malloc(buflen);
			if (buffer == NULL) return buf;
			continue;
		}
		break;
	}
	buf = seed_buf_cat(buf, buffer);
	if (buffer != staticbuf) free(buffer);
	return buf;
}

seed_buf* seed_buf_catf(seed_buf* bufin, const char *fmt, ...) {
	seed_buf* buf = bufin;
	va_list ap;
	va_start(ap, fmt);
		buf = seed_buf_vcatf(buf, fmt, ap);
	va_end(ap);
	return buf;
}

#undef SEED_BUF_MAX_PREALLOC

#ifdef TEST_SEED_BUF

#include "test.h"

$TEST(BUF_CREATE)
$START
	seed_buf* buf;
	buf = seed_buf_create();
	free(buf);
	buf = NULL;
	buf = seed_buf_create_with_string("hello world.");
	if(0 != strcmp("hello world.", buf->data)) {
		$ERROR("value of created seed_buf is out of expectation.")
	}
	free(buf);
	buf = NULL;
$END

$TEST(BUF_RELEASE)
$START
	seed_buf* buf;
	buf = seed_buf_create_with_string("hello seed buf.");
	seed_buf_release(buf);
$END

$TEST(BUF_CAT)
$START
	seed_buf* buf;
	buf = seed_buf_create_with_string("hello");
	if (!!buf) {
		buf = seed_buf_cat(buf, " world.");
		if (0 != strcmp("hello world.", buf->data))
			$ERROR("value of catenated buf is out of expectation, it should be \"hello world.\".")
		buf = seed_buf_cat(buf, "1234567890");
		if (0 != strcmp("hello world.1234567890", buf->data))
			$ERROR("value of catenated buf is out of expectation, it should be \"hello world.1234567890\".")
	}
	seed_buf_release(buf);
$END

$TEST(BUF_CATF)
$START
	seed_buf* buf;
	buf = seed_buf_create_with_string("hello");
	if(!!buf) {
		buf = seed_buf_catf(buf, "%d", 123);
		if (0 != strcmp("hello123", buf->data))
			$ERROR("value of catenated buf is out of expectation.\n\tIt should be \"hello123\".")

		buf = seed_buf_catf(buf, " %s, %s.", "my world", "my girl");
		if (0 != strcmp("hello123 my world, my girl.", buf->data))
			$ERROR("value of catenated buf is out of expectation.\n\tIt should be \"hello123 my world, my girl.\".")
	}
	seed_buf_release(buf);
$END

$TEST(BUF_CATF_LOOP)
$START
	seed_buf* buf;
	buf = seed_buf_create_with_string("hello");
	if (!!buf) {
		int i;
		for (i = 0; i < 100000; i++) {
			buf = seed_buf_catf(buf, "%d", 1);
			buf = seed_buf_catf(buf, "%s", "a");
			buf = seed_buf_catf(buf, "%s", "hahaha, ");
		}
		if (buf->len != 1000005) {
			$ERROR("length of catenated buf is out of expectation.\n\tIt should be 1000005.")
		}
	}
	seed_buf_release(buf);
$END

int main() {
	test_BUF_CREATE();
	test_BUF_RELEASE();
	test_BUF_CAT();
	test_BUF_CATF();
	test_BUF_CATF_LOOP();
	return 0;
}
#endif
