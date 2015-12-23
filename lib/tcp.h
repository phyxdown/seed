#ifndef __SEED_TCP_H
#define __SEED_TCP_H

#include <sys/types.h>

int seed_tcp_dial(const char *host, int port);
void seed_tcp_close(int socketfd);
size_t seed_tcp_write(int socketfd, char *buffer, size_t length);
size_t seed_tcp_read(int socketfd, char *buffer, size_t length);

#endif
