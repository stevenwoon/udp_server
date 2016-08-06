/*
 * main.c
 *
 *  Created on: Sep 4, 2013
 *      Author: steven
 */

/* Sample UDP server */

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define BUFLEN 162
#define NPACK 10
#define PORT 8888

void diep(char *s) {
	perror(s);
	exit(1);
}

int main(void) {
	struct sockaddr_in si_me, si_other;
	int s, i, slen = sizeof(si_other);
	int n;
	uint8_t buf[BUFLEN];

	if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
		diep("socket");

	memset((char *) &si_me, 0, sizeof(si_me));
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(PORT);
	si_me.sin_addr.s_addr = htonl(INADDR_ANY );
	if (bind(s, (struct sockaddr*) &si_me, sizeof(si_me)) == -1)
		diep("bind");
	printf("Starting server\n");

	while (1) {
		printf(".");
		n = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, (socklen_t *) &slen);
		if (n == -1) {
			diep("recvfrom()");
			break;
		}

		printf("Received packet from %s on port %d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));

		for (i = 0; i < n; i++)
			printf("%x ", buf[i]);
		printf("\n");

		int64_t timestamp;
		memcpy(&timestamp, buf+145, 8);

		printf(" Timestamp = %llu\n\n", timestamp);
	}
	printf("---------------------\n");

	close(s);
	return 0;

}

