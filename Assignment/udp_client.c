/**
udp_client.c: The source file of the client in UDP transmission
**/
#include "headsock.h"

void str_cli(FILE *fp, int sockfd, struct sockaddr *addr, int addrlen, long *len);	// function to transmit and receive

void tv_sub(struct timeval *out, struct timeval *in);	// calculate the time interval between out and in

int main(int argc, char *argv[])
{
	// init
	
	// get host's information
	
	// create socket
	
	// print socket information
	
	// transmit and receive
	
	// calculate the average transmission rate
}

void str_cli(FILE *fp, int sockfd, struct sockaddr *addr, int addrlen, long *len)
{
	// init
	
	// allocate memory to contain the whole file
	
	// copy the file into the buffer

	// append the end byte
	// get the current time
	
	// send packets to server
	// stop-and-wait. wait for ACK before transmitting next packet. if NACK, retransmit current packet
	
	// get the current time
	// get the whole transmission time
}

void tv_sub(struct timeval *out, struct timeval *in)
{
	if ((out->tv_usec -= in->tv_usec) < 0)
	{
		--out -> tv_sec;
		out ->tv_usec += 1000000;
	}
	out->tv_usec -= in->tv_sec;
}
