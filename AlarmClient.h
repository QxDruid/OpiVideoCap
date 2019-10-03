#ifndef __ALARMCLIENT__
#define __ALARMCLIENT__

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <sstream>
#include <cstdio>
#include <thread>

#include "TCPClient.h"
#include "Capturer.h"
#include "XMLlist.h"
#include "Button.h"


class AlarmClient
{
public:
	AlarmClient();
	~AlarmClient();
	int start(std::string arg);
	int get_file_size(std::string fname);

    int get_capt();
    int pack_data();
    int send_alarm();
    int send_init_data(); 

private:

    Capturer cap;
    XMLlist xml;   
    TCPClient tcp;
    Button button = 1;
/* Time var */
    std::time_t time;
    std::time_t last_time;
    std::tm* timeinfo;

    /* ctime settings */
    char * format = "%Y-%b-%dT%H-%M-%S";
    char time_buff[50];

/* job with files var*/
    std::ofstream fout;
    std::ifstream fin;
    std::stringstream strStream;
    std::string str;
    
    const char * data_path = "/data/tmp.jpg";

/* FLAGS */

};





#endif
