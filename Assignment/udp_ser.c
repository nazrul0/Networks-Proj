/**
udp_ser.c The source file of the server in UDP transmission
**/
#include "headsock.h"

void str_ser1(int sockfd, struct sockaddr *addr, int addrlen, int *len);	// function to transmit and receive

int main(int argc, char *argv[])
{
	// init
	int len, sockfd;
	struct sockaddr_in my_addr;
	
	// create socket
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		printf("error in socket");
		exit(1);
	}
	
	// bind socket
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(MYUDP_PORT);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(my_addr.sin_zero), 8);
	if (bind(sockfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr)) == -1) {
		printf("error in binding");
		exit(1);
	}
	
	// transmit and receive
	printf("start receiving\n");
	while(1) {
		str_ser(sockfd, (struct sockaddr *)&ser_addr, sizeof(struct sockaddr_in), &len);
	}
	close(sockfd);
	exit(0);
}

void str_ser1(int sockfd, struct sockaddr *addr, int addrlen, int *len)
{
	// init
	char buf[BUFSIZE];
	FILE *fp;
	char recvs[DATALEN];
	int end, n = 0;
	long lseek = 0;
	end = 0;
	struct sockaddr_in src_addr;
	int src_addr_len = sizeof(struct sockaddr_in);
	
	// receive packets
	// simulate random errors. generate random number. if less than ERROR_PROB, ignore and send NACK. if higher, receive and send ACK
	// check if end of file
	printf("receiving data!\n");

	while(!end)
	{
		float error_chance = ((float)rand()/(float)(RAND_MAX)) * 1;
		if (error_chance < ERROR_PROB)
		{
			// ignore and send NACK
			struct ack_so ack;
			ack.num = -1;
			ack.len = 0;

			char ack_buf[sizeof(ack)];
			memcpy(ack_buf, &ack, sizeof(ack));

			sendto(sockfd, &ack_buf, strlen(ack), 0, addr, addrlen); 
		}
		else
		{
			// receive packet and send ACK
			if ((n=recvfrom(sockfd, &recvs, DATALEN, 0, (struct sockaddr *)&src_addr, &src_addr_len)) == -1) {
				printf("error receiving");
				exit(1);
		 	}

			if (recvs[n-1] == '\0') {
				end = 1;
				n --;
			}

			memcpy((buf+lseek), recvs, n);
			lseek += n;

			struct ack_so ack;
			ack.num = 1;
			ack.len = 0;

			char ack_buf[sizeof(ack)];
			memcpy(ack_buf, &ack, sizeof(ack));
			
			sendto(sockfd, &ack_buf, strlen(ack), 0, addr, addrlen);
		}
	}
	
	// write all data received into a file
	if ((fp = fopen("myUDPreceive.txt", "wt")) == NULL)
	{
		printf("File doesn't exist\n");
		exit(0);
	}
	fwrite(buf, 1, lseek, fp);
	fclose(fp);
	printf("A file has been successfully received!\nThe total data received is %d bytes\n", (int)lseek);
}
