#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "buf.h"

Buf bufnew(const char *init) {
	Buf buffer;
	size_t initlen = strlen(init);
	buffer = malloc(sizeof(struct buf) + initlen + 1);
	if (buffer == NULL) return NULL;
	buffer->len = 0;
	buffer->free = initlen;
	buffer = bufcat(buffer, init);
	return buffer;
}

Buf bufcat(Buf buffer, const char *add) {
	size_t addlen = strlen(add);
	if (addlen > buffer->free) {
		size_t newlen = buffer->len + addlen;
		if (newlen < BUF_MAX_PREALLOC) newlen *= 2;
		else newlen += BUF_MAX_PREALLOC;
		buffer = realloc(buffer, sizeof(struct buf)+ newlen + 1);
		if (buffer == NULL) return NULL;
		buffer->free = newlen - buffer->len;
	}
	strcpy(&buffer->data[buffer->len], add);
	buffer->len += addlen;
	buffer->free -= addlen;
	return buffer;
}

Buf bufvcatf(Buf buffer, const char *fmt, va_list ap) {
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
	buffer = bufcat(buffer, buf);
	if (buf != staticbuf) free(buf);
	return buffer;
}

Buf bufcatf(Buf buffer, const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
		buffer = bufvcatf(buffer, fmt, ap);
	va_end(ap);
	return buffer;
}