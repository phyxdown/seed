#include "macros.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>


int seed_net_hostip(char** pp) {
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
		*pp = malloc(strlen(ip)+1);
		strcpy(*pp, ip);
		return 0;
	}
	return -1;
}

int seed_net_hostipx(char** pp) {
	char* ipd;
	int r = seed_net_hostip(&ipd);
	if (r == 0) {
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
		*pp = malloc(strlen(ipx)+1);
		strcpy(*pp, ipx);
		free(ipd);
		return 0;
	} else {
		return -1;
	}
}

#ifdef TEST_SEED_NET
void test_seed_net_hostip() {
	char* s;
	seed_net_hostip(&s);
	printf("%s\n", s);
}
void test_seed_net_hostipx() {
	char* s;
	seed_net_hostipx(&s);
	printf("%s\n", s);
}
int main() {
	test_seed_net_hostip();
	test_seed_net_hostipx();
	return 0;
}
#endif
