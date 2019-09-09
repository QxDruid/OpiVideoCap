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
    if(connect(this->sock, (struct sockaddr *)&this->addr, sizeof(this->addr)) < 0)
    {
        std::cout << "connect error" << std::endl;
        return 0;
    }
    return 1;

}


/*
void TCPClient::Send(const char * data, int len)
{
    char buff[6];
    int transfer_len = 0;
    int sended = 0;

    sprintf(buff, "%06d", len);
    std::cout << "len to send: " << buff << std::endl;

        

        if(send(this->sock, buff, 6, 0) <= 0)
        {
            std::cout << "send len error" << std::endl;
            return;
        }


        while (sended < len)
        {
            if(len >= this->BUFFERSIZE)
                transfer_len = len;

            else
                transfer_len = len - sended;

            if(send(this->sock, data, transfer_len, 0) <= 0)
            {
                std::cout << "send data error" << std::endl;
                return;
            }
            sended += transfer_len;
            data += transfer_len;
        }


        std::cout << len << " bytes sended successfully" << std::endl;

}
*/


int TCPClient::Send(const char * data, int len)
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