#include <iostream>
#include <string>
#include <sstream>


class XMLlist
{
public:
    XMLlist() {
        date_time = '\0';
        id = '\0';
        account = '\0';
        picture = '\0';
        result = '\0';
}

    ~XMLlist() {}

    int len();
    int pack();
    std::string get();

    std::string date_time;
    std::string id;
    std::string account;
    std::string picture;
    std::string result;

private:
    std::stringstream stream;
    std::string list;
};
