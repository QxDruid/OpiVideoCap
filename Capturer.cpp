#include "Capturer.h"

#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"


Capturer::Capturer()
{
    this->capture.release();
}

Capturer::~Capturer()
{
}


int Capturer::init(void)
{
    this->capture.open(0);
	this->capture.set(CV_CAP_PROP_FRAME_WIDTH, 1920);
	this->capture.set(CV_CAP_PROP_FRAME_HEIGHT, 1080);
    this->capture.set(CV_CAP_PROP_BUFFERSIZE, 1);
}


void Capturer::set_path(std::string path_)
{
    this->path = path_;
}

int Capturer::get_capture(void)
{
    cv::Mat image;
    
    std::cout << "Capture is opened" << std::endl;
    if(this->capture.isOpened())
    {
        std::cout << "Get capture" << std::endl;
        this->capture.grab();
        this->capture >> image;

        if(image.empty())
        {
                std::cout << "empty capture" << std::endl;
                return -1;
        }

        imwrite(this->path, image);
       

        return 0;
    }

    else
    {
        std::cout << "No capture" << std::endl;
       // this->image = cv::Mat::zeros(320, 640, CV_8UC1);
        return -1;

    }
}