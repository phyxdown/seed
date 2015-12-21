#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>

char* seed_net_hostip() {
	int sockfd;
	struct ifconf ifconf;
	char buf[1024];
	char* ip;
	struct ifreq *ifreq;
	ifconf.ifc_len = 1024;
	ifconf.ifc_buf = buf;
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		return NULL;
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
		return strdup(ip);
	}
	return NULL;
}

char* seed_net_hostipx() {
	char* ipd;
	if ((ipd = seed_net_hostip()) == NULL) return NULL;
	char ipx[20]; 
	int t[4];
	sscanf(ipd, "%d.%d.%d.%d", &t[0], &t[1], &t[2], &t[3]);
	int i = 0;
	char *ipxp = ipx;
	for (i = 0; i < 4; i++) {
		if (t[i] < 16) {
			sprintf(ipxp+i*2, "0%x", t[i]);
		} else {
			sprintf(ipxp+i*2, "%x", t[i]);
		}
	}
	return strdup(ipx);
}
