#include "TCPClient.h"

    TCPClient::TCPClient()
    {

    }

    TCPClient::~TCPClient()
    {
        close(this->sock);
    }

int TCPClient::init(int port, const char * ip)
{
    
    this->sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
         std::cout << "socket create error" << std::endl;
        return 0;
    }
    this->addr.sin_family = AF_INET;
    this->addr.sin_port = htons(port); 
    this->addr.sin_addr.s_addr = inet_addr(ip);
            while(connect(this->sock, (struct sockaddr *)&this->addr, sizeof(this->addr)) < 0)
    		{
    			std::cout << "try to initial connect" << std::endl;
        		sleep(5);
   			}
    return 1;

}



/*
OLD send func without retry

int TCPClient::Send2(const char * data, int len)
{
    char buff[6];

    sprintf(buff, "%06d", len);
    std::cout << "len to send: " << buff << std::endl;

        if(send(this->sock, buff, 6, MSG_WAITALL) <= 0)
        {
            std::cout << "send len error" << std::endl;
            return 0; 
        }

        if(send(this->sock, data, len, MSG_WAITALL) <= 0)
            {
                std::cout << "send data error" << std::endl;
                return 0;
            }
            std::cout << len << " bytes sended successfully" << std::endl;
            
        return 1;
}

*/

int TCPClient::Send(const char * data, int len)
{
    char buff[6];

    sprintf(buff, "%06d", len);
    std::cout << "len to send: " << buff << std::endl;

        while(send(this->sock, buff, 6, MSG_WAITALL) <= 0)
        {
            sleep(5);
            std::cout << "try to send len" << std::endl;
            while(connect(this->sock, (struct sockaddr *)&this->addr, sizeof(this->addr)) < 0)
    		{
    			close(this->sock);
    			std::cout << "try to reconect 2" << std::endl;
        		sleep(5);
   			}
        }

        while(send(this->sock, data, len, MSG_WAITALL) <= 0)
        {
            sleep(5);
            std::cout << "try to send" << std::endl;
            while(connect(this->sock, (struct sockaddr *)&this->addr, sizeof(this->addr)) < 0)
    		{
    			close(this->sock);
    			std::cout << "try to reconect 2" << std::endl;
        		sleep(5);
   			}
        }
            
        return 1;
}