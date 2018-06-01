/*
 * udp_server.c
 *
 *  Created on: May 30, 2018
 *      Author: yifeifan
 */

#include "unp.h"

void dg_echo(int sockfd, SA *pcliaddr, socklen_t clilen)
{
	int n;
	socklen_t len;
	char mesg[MAXLINE];
	for(;;)
	{
		len = clilen;
		n = recvfrom(sockfd, mesg, MAXLINE, 0, pcliaddr, &len);
		if(n != 0)
		{
			//sendto(sockfd, mesg, n, 0, pcliaddr, len);
			//printf("connection form %s,port %d\n",inet_ntop(AF_INET,&pcliaddr.sin_addr,buff,sizeof(buff)),ntohs(pcliaddr.sin_port));
		}
		sendto(sockfd, mesg, n, 0, pcliaddr, len);
	}
}

int main(int argc, char **argv)
{
	int sockfd;
	struct sockaddr_in servaddr,cliaddr;
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
uint8_t
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_port = htons(SERV_PORT);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_family = AF_INET;
	bind(sockfd, (SA*)&servaddr, sizeof(servaddr));
	dg_echo(sockfd, (SA *)&cliaddr, sizeof(cliaddr));

}
