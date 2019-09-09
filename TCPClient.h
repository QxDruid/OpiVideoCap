#ifndef __TCPCLIENT__
#define __TCPCLIENT__

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <string>
#include <iostream>
#include <sstream>


class TCPClient
{
public:



	TCPClient();
	~TCPClient();

	int Send(const char * data, int len);
	int init(int port, const char * ip);
	//int connect();

private:
	int sock;
    struct sockaddr_in addr;
	
	const int BUFFERSIZE = 1024;
};

#endif