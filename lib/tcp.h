#ifndef __SEED_TCP_H
#define __SEED_TCP_H

#include <sys/types.h>

int tcpDial(const char *host, int port);

void tcpClose(int socketfd);

size_t tcpWrite(int socketfd, char *buffer, size_t length);

size_t tcpRead(int socketfd, char *buffer, size_t length);

#endif
