#ifndef __ALARMCLIENT__
#define __ALARMCLIENT__

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <ctime>
#include <sstream>
#include <cstdio>
#include <thread>
#include <wiringPi.h>
#include <stdlib.h>

#include "TCPClient.h"
#include "XMLlist.h"
#include "Button.h"
#include "MFRC522.h"
#include "Led.h"


class AlarmClient
{
public:
	AlarmClient();
	~AlarmClient();
	int start();
	int get_file_size(std::string fname); // get filesize (use to obtain image size to send)

    int get_capt(); // get screenshot from introscop monitor
    int pack_data(); // prepare xml data and picture to send;
    int pack_xml(); // prepare only xml data (for "turned on" and "user change" signals)
    int send_alarm(); // send alarm signal
    int send_new_user(); // send new user signal
    int send_power_on(); // send power On signal
    int send_power_off();
    int send_init_data();  // send initial data to server (id, user) after connection (to set device as alarm client)
    int load_config(); // load config and user list flom files

private:

    XMLlist xml;   
    TCPClient tcp;
    Button button = 1;
    Button power_on = 11;
    MFRC522 card_reader;
    Led led;
    bool is_on = 0;

/* user definition var */
    std::map<std::string, std::string> usermap; // Map of all users (load from users file)
    std::string current_user = "indefinite user"; // initial set user as default
    std::string id; // id of devise (load from config file)
    std::string gateway; // ip of server (load from config file)

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
    std::string str;  // string to send file
    
    const char * data_path = "/data/tmp.jpg";
    const char * CONFIG = "/etc/alarm-client/config";
    const char * USERS = "/etc/alarm-client/users";

/* FLAGS */

};





#endif
