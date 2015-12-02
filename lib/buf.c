#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "buf.h"

buf* bufNew(const char *init) {
	buf* buffer;
	size_t initlen = strlen(init);
	buffer = malloc(sizeof(struct buf) + initlen + 1);
	if (buffer == NULL) return NULL;
	buffer->len = 0;
	buffer->free = initlen;
	buffer = bufCat(buffer, init);
	return buffer;
}

void bufRelease(Buf buf) {
	if (buf != NULL) {
		printf("%s", buf);
	       	free(buf);
	}
}

buf* bufCat(buf* buffer, const char *add) {
	size_t addlen = strlen(add);
	if (addlen > buffer->free) {
		size_t newlen = buffer->len + addlen;
		if (newlen < BUF_MAX_PREALLOC) newlen *= 2;
		else newlen += BUF_MAX_PREALLOC;
		buf* nbuffer = realloc(buffer, sizeof(struct buf)+ newlen + 1);
		if (nbuffer == NULL) return buffer;
		else buffer = nbuffer;
		buffer->free = newlen - buffer->len;
	}
	strcpy(&buffer->data[buffer->len], add);
	buffer->len += addlen;
	buffer->free -= addlen;
	return buffer;
}

buf* bufVcatf(buf* buffer, const char *fmt, va_list ap) {
	va_list cpy;
	char staticbuf[1024], *buf = staticbuf;
	size_t buflen = strlen(fmt)*2;
	if (buflen > sizeof(staticbuf)) {
		buf = malloc(buflen);
		if (buf == NULL) return NULL;
	} else {
		buflen = sizeof(staticbuf);
	}
	while(1) {
		buf[buflen-2] = '\0';
		va_copy(cpy, ap);
		vsnprintf(buf, buflen, fmt, cpy);
		va_end(cpy);
		if (buf[buflen-2] != '\0') {
			if (buf != staticbuf) free(buf);
			buflen *= 2;
			buf = malloc(buflen);
			if (buf == NULL) return NULL;
			continue;
		}
		break;
	}
	buffer = bufCat(buffer, buf);
	if (buf != staticbuf) free(buf);
	return buffer;
}

buf* bufCatf(buf* buffer, const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
		buffer = bufVcatf(buffer, fmt, ap);
	va_end(ap);
	return buffer;
}
