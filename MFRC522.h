#ifndef __MFRC522__
#define __MFRC522__

#include <wiringPi.h>
#include <wiringSerial.h>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <iostream>
#include <map>



class MFRC522
{
public:
	MFRC522();
	~MFRC522();

	int data_available(void);
	void init();
	std::string read();

private:
	const char * serial = "/dev/ttyS2";
	const int IRQ_PIN = 4;

	int usart;

};


#endif