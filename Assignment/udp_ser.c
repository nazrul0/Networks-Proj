/**
udp_ser.c The source file of the server in UDP transmission
**/
#include "headsock.h"

void str_ser1(int sockfd);	// function to transmit and receive

int main(int argc, char *argv[])
{
	// init
	
	// create socket
	
	// bind socket
	
	// transmit and receive
}

void str_ser1(int sockfd)
{
	// init
	
	// receive packets
	// simulate random errors. generate random number. if less than ERROR_PROB, ignore and send NACK. if higher, receive and send ACK
	// check if end of file
	
	// write all data received into a file
}
