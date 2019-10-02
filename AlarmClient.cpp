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
    this->cap.set_path("data/tmp.jpg");
    this->cap.init();

    /* static xml data */
    this->xml.account = "Katya";
    this->xml.id = "003";
    this->xml.result = "detected";


    /* TCPClient connection init */
    this->tcp.init(3425, arg.c_str());
    this->tcp.Send("Opi", 3);
    this->tcp.Send(this->xml.id.c_str(), 3);
    this->tcp.Send(this->xml.account.c_str(), this->xml.account.length());

    last_time = std::time(NULL);


    while(1)
    {
        if(button.isPressed())
        {
            this->strStream.str("");
            this->str = "";
            /* get capture */
            remove("data/tmp.jpg");

            int len = 0;
            do
            {
                this->cap.get_capture();
                len = get_file_size("data/tmp.jpg");
            } while (len < 150000); // val to EZCAP -> 100000;

            /*get current time*/
            this->time = std::time(NULL);
            this->timeinfo = std::localtime(&this->time);
            std::strftime(this->time_buff, 50, this->format, this->timeinfo);

            /* send XMLpacket*/
            this->xml.date_time = std::string(this->time_buff);
            std::cout << "datetime =" << this->xml.date_time << std::endl;
            this->xml.picture = std::string(this->time_buff)  + ".jpg";
            this->xml.pack();

            /* send header and file */
            this->tcp.Send("xml", 3);

  
            this->tcp.Send((this->xml.date_time + ".xml").c_str(), 24);
            //tcp.Send(str.c_str(), str.size());
            this->tcp.Send(const_cast<char*>(this->xml.get().c_str()), this->xml.len());
            
            this->fout.open("data/tmp2.jpg");
            if(!this->fout.is_open())
            {
                std::cout << "TMP2 NOT OPEND" << std::endl;
            }
            /*image send*/
            this->fin.open("data/tmp.jpg");
            this->strStream.str(""); // clear sstream
            this->strStream << this->fin.rdbuf();
            this->str = this->strStream.str();
            this->fin.close();
            
            this->fout << this->str;
            this->fout.close();
            
            this->tcp.Send("image", 5);
            this->tcp.Send(this->str.c_str(), this->str.size());
        }
    }

    exit(EXIT_SUCCESS);
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