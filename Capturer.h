#ifndef CAPTURER_H
#define CAPTURER_H

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include <string>
#include <iostream>
#include <fstream>

class Capturer
{
public:
    Capturer();
    ~Capturer();
    void set_path(std::string path_);
    int get_capture(void);
    int init(void);

private:
    std::string path;
    cv::VideoCapture capture;
};


#endif
