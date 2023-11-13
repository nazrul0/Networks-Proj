/**
udp_client.c: The source file of the client in UDP transmission
**/
#include "headsock.h"

float str_cli(FILE *fp, int sockfd, struct sockaddr *addr, int addrlen, long *len);	// function to transmit and receive

void tv_sub(struct timeval *out, struct timeval *in);	// calculate the time interval between out and in

int main(int argc, char *argv[])
{
	// init
	int sockfd;
	long len;
	struct sockaddr_in ser_addr;
	char **pptr;
	struct hostent *sh;
	struct in_addr **addrs;
	FILE *fp;
	float ti, rt, tp;

	if (argc != 2)
	{
		printf("parameters invalid");
		exit(0);
	}
	
	// get host's information
	if ((sh=gethostbyname(argv[1]))==NULL) {
		printf("error when getbyhostname");
		exit(0);
	}
	
	// create socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0)
	{
		printf("error in socket");
		exit(1);
	}
	
	addrs = (struct in_addr **)sh->h_addr_list;
	printf("canonical name: %s\n", sh->h_name);
	for (pptr=sh->h_aliases; *pptr != NULL; pptr++) {
		printf("the alias' name is: %s\n", *pptr);	// print socket information
	}
	switch(sh->h_addrtype)
	{
		case AF_INET:
			printf("AF_INET\n");
		break;
		default:
			printf("unknown addrtype\n");
		break;
	}

	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(MYUDP_PORT);
	memcpy(&(ser_addr.sin_addr.s_addr), *addrs, sizeof(struct in_addr));
	bzero(&(ser_addr.sin_zero), 8);
	
	// transmit and receive
	if ((fp = fopen("myfile.txt", "r+t")) == NULL)
	{
		printf("File doesn't exist\n");
		exit(0);
	}
	ti = str_cli(fp, sockfd, (struct sockaddr *)&ser_addr, sizeof(struct sockaddr_in), &len);
	rt = (len/(float)ti);
	tp = rt*8/(float)1000;
	
	// calculate the transmission data
	printf("Time (ms): %.3f, Data Sent (bytes): %d\nData rate: %f (Kbytes/s), Throughput (Mbps): %f\n", ti, (int)len, rt, tp);
	
	close(sockfd);
	fclose(fp);
	exit(0);
}

float str_cli(FILE *fp, int sockfd, struct sockaddr *addr, int addrlen, long *len)
{
	// init
	char *buf;
	long lsize, ci;
	char sends[DATALEN];
	struct ack_so ack;
	int n, slen;
	float time_inv = 0.0;
	struct timeval sendt, recvt;
	ci = 0;

	fseek(fp , 0 , SEEK_END);
	lsize = ftell (fp);
	rewind(fp);
	printf("The file length is %d bytes\n", (int)lsize);
	printf("the packet length is %d bytes\n", DATALEN);
	
	// allocate memory to contain the whole file
	buf = (char *) malloc (lsize+1);
	if (buf == NULL) exit(2);
	
	// copy the file into the buffer
	fread(buf, 1, lsize, fp);

	buf[lsize] = '\0';	// append the end byte
	gettimeofday(&sendt, NULL);	// get the current time

	printf("before. ci: %ld. lsize: %ld\n", ci, lsize);
	// send packets to server
	// stop-and-wait. wait for ACK before transmitting next packet. if NACK, retransmit current packet
	while(ci <= lsize)
	{
		printf("top. ci: %ld. lsize: %ld\n", ci, lsize);
		if ((lsize+1-ci) <= DATALEN)
			slen = lsize+1-ci;
		else
			slen = DATALEN;
		memcpy(sends, (buf+ci), slen);
		n = sendto(sockfd, &sends, slen, 0, addr, addrlen);
		if (n == -1)
		{
			printf("send error!");
			exit(1);
		}

		if ((n = recvfrom(sockfd, &ack, 2, 0, addr, (socklen_t*)&addrlen)) == -1) {
			printf("error when receiving ACK");
			exit(1);
		}

		if (ack.num == -1) {
			// n = sendto(sockfd, &sends, strlen(sends), 0, addr, addrlen);
			printf("NACK received");
		} else {
			ci += slen;
			printf("ACK received");
		}
	}
	printf("after. ci: %ld. lsize: %ld\n", ci, lsize);
	
	// get the current time
	// get the whole transmission time
	gettimeofday(&recvt, NULL);
	*len = ci;
	tv_sub(&recvt, &sendt);
	time_inv += (recvt.tv_sec)*1000.0 + (recvt.tv_usec)/1000.0;
	return(time_inv);
}

void tv_sub(struct timeval *out, struct timeval *in)
{
	if ((out->tv_usec -= in->tv_usec) < 0)
	{
		--out -> tv_sec;
		out ->tv_usec += 1000000;
	}
	out->tv_sec -= in->tv_sec;
}
