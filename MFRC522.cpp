#include "MFRC522.h"

MFRC522::MFRC522()
{
	
}
MFRC522::~MFRC522()
{
	
}

void MFRC522::init()
{
	pinMode(this->IRQ_PIN, INPUT);
}

int MFRC522::data_available(void)
{
	if(digitalRead(this->IRQ_PIN) == 0)	
		return 1;

	else
		return 0;
}


std::string MFRC522::read()
{
	char tmp;
	std::string str = "";
	int count = 0;

	/* open and close serial while reading to avoid noise parsels durind waiting */
	if ((usart = serialOpen (this->serial, 4800)) < 0)
  	{
    	std::cout << "Unable to open serial device" <<std::endl;
   		return "";
  	}
	//serialGetchar(usart); // два первых байта в мусор для устаноки линии usart
	//serialGetchar(usart);

	while(count < 12)
  	{		
  		tmp = serialGetchar(usart);
  		str += tmp;
  		++count;
  	}

  	serialFlush(usart);
  	close(usart);

    return str;
}