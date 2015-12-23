#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "tcp.h"

int seed_tcp_dial(const char *host, int port) {
	int    sockfd;
	struct sockaddr_in servaddr;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return -1;

	memset(&servaddr, '0', sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	if (inet_pton(AF_INET, host, &servaddr.sin_addr) < 0)
		return -1;

	if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
		return -1;
	return sockfd;
}

void seed_tcp_close(int sockfd) {
	close(sockfd);
}

size_t seed_tcp_write(int sockfd, char *buffer, size_t length) {
	return write(sockfd, buffer, length);
}

size_t seed_tcp_read(int sockfd, char *buffer, size_t length) {
	return read(sockfd, buffer, length);
}

int main() {
	close(-1);
}
