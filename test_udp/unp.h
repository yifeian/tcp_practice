/*
 * unp.h
 *
 *  Created on: May 22, 2018
 *      Author: yifeifan
 */

#ifndef UNP_H_
#define UNP_H_
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/un.h>
#include <sys/time.h>
#include <sys/select.h>


#define SERV_PORT 9877
#define SA struct sockaddr
#define LISTENQ   1024
#define	MAXLINE		4096	/* max text line length */
#define	BUFFSIZE	8192	/* buffer size for reads and writes */
#define	max(a,b)	((a) > (b) ? (a) : (b))
typedef struct
{
	long arg1;
	long arg2;
}args;

typedef struct
{
	long sum;
}result;
typedef	void	Sigfunc(int);
Sigfunc *Signal(int, Sigfunc *);
ssize_t readn(int filedes, void *buff, size_t nbytes);
ssize_t written(int filedes, const void *buff, size_t nbytes);
ssize_t readline(int filedes, void *buff, size_t maxlen);

void sig_chld(int signo);
#endif /* UNP_H_ */
