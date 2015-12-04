#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/types.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>

int seed_net_GetHostIP(char* ret) {
	int sockfd;
	struct ifconf ifconf;
	char buf[1024];
	char* ip;
	struct ifreq *ifreq;
	ifconf.ifc_len = 1024;
	ifconf.ifc_buf = buf;
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		return -1;
	}
	ioctl(sockfd, SIOCGIFCONF, &ifconf);
	close(sockfd);

	ifreq = (struct ifreq*)buf;
	int i;
	for (i = (ifconf.ifc_len/sizeof(struct ifreq)); i > 0; i--) {
		ip = inet_ntoa(((struct sockaddr_in*)&(ifreq->ifr_addr))->sin_addr);
		if (strcmp(ip, "127.0.0.1") == 0) { 
			ifreq++;
			continue;
		}
		strcpy(ret, ip);
		return 0;
	}
	return -1;
}
