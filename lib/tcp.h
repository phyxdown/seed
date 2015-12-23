#ifndef __SEED_TCP_H
#define __SEED_TCP_H

#include <sys/types.h>

int seed_tcp_dial(const char *host, int port);
void seed_tcp_close(int socketfd);
int seed_tcp_write(int socketfd, char *buffer, int length);
int seed_tcp_read(int socketfd, char *buffer, int length);

#endif
