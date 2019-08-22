#include "XMLlist.h"



int XMLlist::len()
{
    return this->list.length();
}

int XMLlist::pack()
{
   this->stream<< "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << std::endl
              << "<tt:Message UtcTime=\" " << this->date_time << "\"" << std::endl
              << "PropertyOperation=\"Initialized\" xmlns:tt=\"http://www.onvif.org/ver10/schema\">" << std::endl
              << "<tt:Source>"  << std::endl
              << "<tt:SimpleItem Name=\"Id\" Value=\"" << this->id << "\" />" << std::endl
              << "</tt:Source>" << std::endl << "<tt:Key />" << std::endl << "<tt:Data>" << std::endl
              << "<tt:SimpleItem Name=\"Account\" Value=\"" << this->account << "\" />" << std::endl
              << "<tt:SimpleItem Name=\"Picture\" Value=\"" << this->picture << "\" />" << std::endl
              << "<tt:SimpleItem Name=\"Result\" Value=\""<< this->result << "\" />" << std::endl
              << "</tt:Data>" << std::endl << "</tt:Message>" << std::endl;

    this->list = this->stream.str();
    this->stream.str("");
    return 1;
}

std::string XMLlist::get()
{
    return this->list;
}
