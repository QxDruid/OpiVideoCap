#include "AlarmClient.h"

AlarmClient::AlarmClient()
{
    wiringPiSetup();
}

AlarmClient::~AlarmClient()
{

}


int AlarmClient::load_config()
{
/* load config */
    std::ifstream config(this->CONFIG);
    if(!config.is_open())
    {
        std::cout << "config cant be opened" << std::endl;
        return -1;
    }
    
    std::string tmp1,tmp2;

    while(config >> tmp1 >> tmp2)
    {
        if(tmp1 == "id:")
            this->id = tmp2;
        else if(tmp1 == "gateway:")
            this->gateway = tmp2;
    }

    config.close();

/* Load users */
    config.open(this->USERS);
    if(!config.is_open())
    {
        std::cout << "config cant be opened" << std::endl;
        return -1;
    }
    
    std::string tmp;
    const std::string delimiter = "=";
    
    while(getline(config, tmp))
    {
            /* split string tmp by spliter '=' */
        std::string::size_type start = 0;
        std::string::size_type finish = 0;
            /* find first part */
        finish = tmp.find(delimiter, start);
        tmp1 = tmp.substr(start, finish-start);
            /* move start to end of first part */
        start = finish + delimiter.size();
            /* find first part */
        finish = tmp.find(delimiter, start);
        tmp2 = tmp.substr(start, finish-start);
        this->usermap.insert ( std::pair<std::string, std::string>(tmp2,tmp1) );
    }
        
    
    config.close();
    return 0;
}


int AlarmClient::start()
{
    this->load_config();
    this-> led.init();
    /* Capturer image save path */
    this->cap.set_path(data_path);
    this->cap.init();

    /* static xml data */
    this->xml.account = this->current_user;
    this->xml.id = this->id;
    this->xml.result = "devise started";

    /* TCPClient connection init */
    this->tcp.init(3425, this->gateway.c_str());
    this->send_init_data();

    last_time = std::time(NULL);

    std::string tmp_user;
    int err = 0;
    while(1)
    {
        usleep(50000);


        /* if user press alarm button */
        if(button.isPressed())
        {
            this->get_capt();

            while(this->send_alarm() < 0)
            {
                this->led.Error(ON);

                std::cout << "try to reconnect" << std::endl;
                while(tcp.reconnect() < 0)
                {
                    sleep(5);
                    std::cout << "try to reconnect" << std::endl;
                }
                std::cout << "reconnected" << std::endl;
                this->send_init_data(); 
            }
            this->led.Error(OFF);
            this->led.Ok(ON);
            sleep(1);
            this->led.Off();
            std::cout << "sended success" << std::endl;    
        }

        /* If new user insert his persanal RFID card */
        if(this->card_reader.data_available())
        {
            tmp_user = this->card_reader.read();
            if(this->usermap.find(tmp_user) != usermap.end())
            {
                this->led.Ok(ON);
                tmp_user = usermap[tmp_user];
                if(tmp_user != this->current_user)
                {
                                        // Green led On
                    this->current_user = tmp_user;
                }
                else
                {

                    this->current_user = "indefinite user";
                }
            } 
            else
            {
                this->led.Error(ON);
                this->current_user = "indefinite user";   // --------------------------------------------> вставить зажигание красной или зеленой лампочки о входе <---------
            }
            

            while(this->send_new_user() < 0)
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

            sleep(1);
            this->led.Off();

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


int AlarmClient::pack_xml()
{
    this->strStream.str("");
    this->str = "";

    /*get current time*/
    this->time = std::time(NULL);
    this->timeinfo = std::localtime(&this->time);
    std::strftime(this->time_buff, 50, this->format, this->timeinfo);

    /*  XMLpacket*/
    this->xml.account = this->current_user;
    this->xml.date_time = std::string(this->time_buff);
    std::cout << "datetime =" << this->xml.date_time << std::endl;
    this->xml.picture = "no_picture";
    this->xml.pack();
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
            this->xml.account = this->current_user;
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
    // prepare data to send
    this->xml.result = "detected";
    this->pack_data();

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



int AlarmClient::send_new_user()
{
    // prepare data to send
    this->xml.result = "user changed";
    this->pack_xml();

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
