/*
 * ntp.cpp
 *
 *  Created on: May 23, 2018
 *      Author: yifeifan
 */
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#define JAN_1970      0x83aa7e80
#define LI 0
#define VN 3
#define MODE 3
#define STRATUM 0
#define POLL 0
#define PREC 0

typedef struct NTPPACKET
{
  uint8_t     li_vn_mode;
  uint8_t     stratum;
  uint8_t    poll;
  uint8_t     precision;
  uint32_t   root_delay;
  uint32_t   root_dispersion;
  int8_t      ref_id[4];
  uint32_t   reftimestamphigh;
  uint32_t   reftimestamplow;
  uint32_t   oritimestamphigh;
  uint32_t   oritimestamplow;
  uint32_t   recvtimestamphigh;
  uint32_t   recvtimestamplow;
  uint32_t   trantimestamphigh;
  uint32_t   trantimestamplow;
}NTPPacket;
NTPPacket ntp_recv_data;
int32_t   firsttimestamp,finaltimestamp;
int32_t   diftime,delaytime;
#define NTPFRAC(x) (4294 * (x) + ((1981 * (x))>>11))
#define USEC(x) (((x) >> 12) - 759 * ((((x) >> 10) + 32768) >> 16))

int ntpdate(int argc, char **argv);

void NTPPacket_ntohl(unsigned int *data)
{
	uint8_t i;
    for(i = 0; i < 12; i++)
    {
    	printf("recv data is %u\n",data[i]);
    }
	ntp_recv_data.reftimestamphigh = ntohl(((unsigned int *)data)[4]);
	ntp_recv_data.recvtimestamplow = ntohl(((unsigned int *)data)[5]);
	ntp_recv_data.oritimestamphigh = ntohl(((unsigned int *)data)[6]);
	ntp_recv_data.oritimestamplow = ntohl(((unsigned int *)data)[7]);
	ntp_recv_data.recvtimestamphigh = ntohl(((unsigned int *)data)[8]);
	ntp_recv_data.recvtimestamplow = ntohl(((unsigned int *)data)[9]);
	ntp_recv_data.trantimestamphigh = ntohl(((unsigned int *)data)[10]);
	ntp_recv_data.trantimestamplow = ntohl(((unsigned int *)data)[11]);
}

int main(int argc, char **argv)
{
    ntpdate(argc, argv);
    return 0;
}



void ntp_init(unsigned int *data)
{
	struct timeval tv;
	uint8_t i;
	/*
	memset(&ntp_tran_data,0,sizeof(ntp_tran_data));
	gettimeofday(&tv,NULL);
	printf("now seconds is  %ld\n",tv.tv_sec);
	ntp_tran_data.li_vn_mode = 19;
	firsttimestamp = tv.tv_sec + 0x83aa7e80;
	ntp_tran_data.oritimestamphigh = htonl(firsttimestamp);
	//NTPPacket_htonl(&ntp_tran_data);
	 *
	 */

    data[0] = htonl((LI << 30) | (VN << 27) | (MODE << 24)
                  | (STRATUM << 16) | (POLL << 8) | (PREC & 0xff));
    gettimeofday(&tv, NULL);
    data[10] = htonl(tv.tv_sec + JAN_1970); /* Transmit Timestamp coarse */
    data[11] = htonl(NTPFRAC(tv.tv_usec));  /* Transmit Timestamp fine   */
    for(i = 0; i < 12; i++)
    {
    	printf("send data is %u\n",data[i]);
    }
}

int ntpdate(int argc, char **argv)
{
	//char *hostname=(char *)"163.117.202.33";
	//char *hostname=(char *)"pool.ntp.br";
	char    *hostname=(char *)"223.65.211.46";
	int portno=123;     //NTP is port 123
	int i;          // misc var i
	//ntp_packet combinedate;
	struct timeval tv;

	//unsigned char msg[48]={19,0,0,0,0,0,0,0,0};    // the packet we send
	unsigned int trans_data[12];
	unsigned int recv_data[12];
	//struct in_addr ipaddr;        //
	struct protoent *proto;     //
	struct sockaddr_in server_addr;
	int s;  // socket
	long tmit;   // the time -- This is a time_t sort of

	//use Socket;
	//
	//#we use the system call to open a UDP socket
	//socket(SOCKET, PF_INET, SOCK_DGRAM, getprotobyname("udp")) or die "socket: $!";
	if(argc != 2)
	{
		printf("usage: tcpcli < ipaddress");
		return -1;
	}
	proto=getprotobyname("udp");

	s=socket(PF_INET, SOCK_DGRAM, proto->p_proto);
	perror("socket");
	//
	//#convert hostname to ipaddress if needed
	//$ipaddr   = inet_aton($HOSTNAME);
	memset( &server_addr, 0, sizeof( server_addr ));
	server_addr.sin_family=AF_INET;
	//server_addr.sin_addr.s_addr = inet_addr(hostname);
	inet_pton(AF_INET, argv[1],&server_addr.sin_addr);
	//argv[1] );
	//i   = inet_aton(hostname,&server_addr.sin_addr);
	server_addr.sin_port=htons(portno);
	//printf("ipaddr (in hex): %x\n",server_addr.sin_addr);
	memset(trans_data, 0, sizeof(trans_data));
	memset(recv_data, 0, sizeof(recv_data));
	ntp_init(trans_data);
	printf("sending data..\n");
	i=sendto(s,trans_data,sizeof(trans_data),0,(struct sockaddr *)&server_addr,sizeof(server_addr));
	perror("sendto");
	// get the data back
	struct sockaddr saddr;
	socklen_t saddr_l = sizeof (saddr);
	i=recvfrom(s,recv_data,48,0,&saddr,&saddr_l);
	perror("recvfr:");
	NTPPacket_ntohl(recv_data);
	gettimeofday(&tv,NULL);
	finaltimestamp=tv.tv_sec+0x83aa7e80;
	//offset=（（T2-T1）+（T3-T4））/2=小时
	diftime=((ntp_recv_data.recvtimestamphigh-ntp_recv_data.oritimestamphigh)+(ntp_recv_data.trantimestamphigh-finaltimestamp))/2;
	//Delay=（T4-T1）-（T3-T2）/2 =秒
	delaytime=((finaltimestamp-ntp_recv_data.oritimestamphigh)-(ntp_recv_data.trantimestamphigh-ntp_recv_data.recvtimestamphigh));
	gettimeofday(&tv,NULL);
 	tv.tv_sec=tv.tv_sec+diftime+delaytime;
	tmit = tv.tv_sec;

	std::cout << "time is " << ctime(&tmit)  << std::endl;
	i=time(0);
	//printf("%d-%d=%d\n",i,tmit,i-tmit);
	//printf("System time is %d seconds off\n",(i-tmit));
	std::cout << "System time is " << (i-tmit) << " seconds off" << std::endl;
	return 0;
}



