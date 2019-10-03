#include "AlarmClient.h"

AlarmClient::AlarmClient()
{

}

AlarmClient::~AlarmClient()
{

}

int AlarmClient::start(std::string arg)
{

    /* Capturer image save path */
    this->cap.set_path(data_path);
    this->cap.init();

    /* static xml data */
    this->xml.account = "Katya";
    this->xml.id = "003";
    this->xml.result = "detected";

    /* TCPClient connection init */
    this->tcp.init(3425, arg.c_str());
    this->send_init_data();

    last_time = std::time(NULL);

    int err = 0;
    while(1)
    {
        usleep(50000);
        if(button.isPressed())
        {
            this->get_capt();
            this->pack_data();

            while(this->send_alarm() < 0)
            {
                std::cout << "try to reconnect" << std::endl;
                while(tcp.reconnect() < 0)
                {
                    sleep(5);
                    std::cout << "try to reconnect" << std::endl;
                }
                std::cout << "reconnected" << std::endl;
                this->send_init_data(); 
            }
            std::cout << "sended success" << std::endl;    
        }
    }

    exit(EXIT_SUCCESS);
}


int AlarmClient::get_capt()
{
    /* get capture */
    remove(data_path);

    int err = 0;
    int len = 0;

    do
    {
        err = this->cap.get_capture();
        if(err < 0)
        {
            std::cout << "get capture error" << std::endl;
            continue;
        }

        len = get_file_size(data_path);
    } while (len < 100000); // val to EZCAP -> 100000;

    /* if success */
    return 0;
}

int AlarmClient::pack_data()
{

            this->strStream.str("");
            this->str = "";

            /*get current time*/
            this->time = std::time(NULL);
            this->timeinfo = std::localtime(&this->time);
            std::strftime(this->time_buff, 50, this->format, this->timeinfo);

            /*  XMLpacket*/
            this->xml.date_time = std::string(this->time_buff);
            std::cout << "datetime =" << this->xml.date_time << std::endl;
            this->xml.picture = std::string(this->time_buff)  + ".jpg";
            this->xml.pack();

            /*convert image to string for send*/
            this->fin.open(data_path);
            this->strStream.str(""); // clear sstream
            this->strStream << this->fin.rdbuf();
            this->str = this->strStream.str();
            this->fin.close();

            return 0;
}


int AlarmClient::send_init_data()
{
    this->tcp.Send("Opi", 3);
    this->tcp.Send(this->xml.id.c_str(), 3);
    this->tcp.Send(this->xml.account.c_str(), this->xml.account.length());
}

int AlarmClient::send_alarm()
{

             
    int err = 0;

    /* ------------- SEND XML ------------*/
    /* send info message (that we send XML) */
    err = this->tcp.Send("xml", 3);
    if(err < 0)
        return -1;
    /* send info message (file name) */
    err = this->tcp.Send((this->xml.date_time + ".xml").c_str(), 24);
    if(err < 0)
        return -1;
    /* send XML file*/
    err = this->tcp.Send(const_cast<char*>(this->xml.get().c_str()), this->xml.len());
    if(err < 0)
        return -1;
            
    /* ------------- SEND IMG ------------*/
    /* send info message (that we send image) */
    err = this->tcp.Send("image", 5);
    if(err < 0)
        return -1;
    /* send image */
    err = this->tcp.Send(this->str.c_str(), this->str.size());
    if(err < 0)
        return -1;
    /* if sending success */
    return 0;
}


int AlarmClient::get_file_size(std::string fname)
{
    std::fstream file(fname);
    int size = 0;
    file.seekg (0, std::ios_base::end);
    size = file.tellg();
    file.seekg(0,std::ios_base::beg);

    file.close();
    return size;
}
