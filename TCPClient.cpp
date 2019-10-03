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

int TCPClient::reconnect()
{
    close(this->sock);

    this->sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
         std::cout << "socket create error" << std::endl;
        return 0;
    }
    if(connect(this->sock, (struct sockaddr *)&this->addr, sizeof(this->addr)) < 0)
        return -1;
    else
        return 0;
}


int TCPClient::Send(const char * data, int len)
{
    char buff[6];
    int send_err = 0;
    std::string data_str;

    sprintf(buff, "%06d", len);
    std::cout << "len to send: " << buff << std::endl;

    send_err = send(this->sock, buff, 6, MSG_WAITALL);
    if(send_err < 0)    
        std::cout << "len send err" << std::endl;
    else if(send_err == 0)
        std::cout << "socket closed (len)" << std::endl;

    send_err = send(this->sock, data, len, MSG_WAITALL);
    if(send_err < 0)    
        std::cout << "data send err" << std::endl;
    else if(send_err == 0)
        std::cout << "socket closed (data)" << std::endl;

    data_str = this->recv_data(this->sock);
    if(data_str.empty())
        std::cout << "Socket disconnected" << std::endl;
    else if(data_str == "Ok")
        return 0;
    return -1;
}


std::string TCPClient::recv_data(int sock)
{
        char len_buffer[6]; // buffer to collect data size
        char * recv_buffer;
        int len_to_recv = 0;
        int bytes_read = 0;

        bytes_read = recv(sock, len_buffer, 6, MSG_WAITALL);
        if(bytes_read <= 0)
        {
            std::cout << "len recv failed" << std::endl;
            return "";
        }
        sscanf(len_buffer, "%d", &len_to_recv); // translate char* to int


        recv_buffer = new char[len_to_recv];
            
        bytes_read = recv(sock, recv_buffer, len_to_recv, MSG_WAITALL);
        if(bytes_read <= 0)
        {
            std::cout << "data recv failed" << std::endl;
            return "";
        }

        std::string str_data = std::string(recv_buffer, len_to_recv);
        return str_data;
}
