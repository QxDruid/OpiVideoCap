#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <sstream>
#include <cstdio>

#include "TCPClient.h"
#include "Capturer.h"
#include "XMLlist.h"
#include "Button.h"

int get_file_size(std::string fname);

int main(int argc,char ** argv)
{
    if(argc < 2)
    {
        std::cout << "example: sudo ./spam 192.168.10.21" << std::endl;
        return 0;
    }



    Capturer cap;
    XMLlist xml;   
    TCPClient tcp;
    int len = 0;
    /* Capturer image save path */
    cap.set_path("data/tmp.jpg");
    cap.init();

    Button button(1);
    /* static xml data */
    xml.account = "Katya";
    xml.id = "003";
    xml.result = "detected";

    /* ctime settings */
    std::time_t time;
    std::time_t last_time;
    std::tm* timeinfo;
    char * format = "%Y-%b-%dT%H-%M-%S";
    char time_buff[50];

    /* TCPClient connection init */
    tcp.init(3425, argv[1]);
    tcp.Send("Opi", 3);
    tcp.Send(xml.id.c_str(), 3);
    tcp.Send(xml.account.c_str(), xml.account.length());

    last_time = std::time(NULL);

    std::ofstream fout;
    std::ifstream fin;
    std::stringstream strStream;
    std::string str;
    while(1)
    {
        if(button.isPressed())
        {
            strStream.str("");
            str = "";
            /* get capture */
            remove("data/tmp.jpg");
            do
            {
                cap.get_capture();
                len = get_file_size("data/tmp.jpg");
            } while (len < 150000); // val to EZCAP -> 100000;

            /*get current time*/
            time = std::time(NULL);
            timeinfo = std::localtime(&time);
            std::strftime(time_buff, 50, format, timeinfo);

            /* send XMLpacket*/
            xml.date_time = std::string(time_buff);
            std::cout << "datetime =" << xml.date_time << std::endl;
            xml.picture = std::string(time_buff)  + ".jpg";
            xml.pack();


            /* send header and file */
            tcp.Send("xml", 3);
            tcp.Send((xml.date_time + ".xml").c_str(), 24);
            //tcp.Send(str.c_str(), str.size());
            tcp.Send(const_cast<char*>(xml.get().c_str()), xml.len());
            

            fout.open("data/tmp2.jpg");
            if(!fout.is_open())
            {
                std::cout << "TMP2 NOT OPEND" << std::endl;
            }
            /*image send*/
            fin.open("data/tmp.jpg");
            strStream.str(""); // clear sstream
            strStream << fin.rdbuf();
            str = strStream.str();
            fin.close();
            
            fout << str;
            fout.close();
            
            tcp.Send("image", 5);
            tcp.Send(str.c_str(), str.size());
        }
    }

    return 0;
}

int get_file_size(std::string fname)
{
    std::fstream file(fname);
    int size = 0;
    file.seekg (0, std::ios_base::end);
    size = file.tellg();
    file.seekg(0,std::ios_base::beg);

    file.close();
    return size;
}