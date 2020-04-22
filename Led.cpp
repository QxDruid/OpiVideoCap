#include "Led.h"

Led::Led()
{
	
}

Led::~Led()
{

}

void Led::init()
{
	pinMode(this->led_ok, OUTPUT);
	pinMode(this->led_error, OUTPUT);
	this->Off();
}

void Led::Ok(bool status)
{
	if(status)
		digitalWrite(this->led_ok, 1);
	else
		digitalWrite(this->led_ok, 0);

	return;
}

void Led::Off()
{
	digitalWrite(this->led_ok, 0);
	digitalWrite(this->led_error, 0);
}

void Led::Error(bool status)
{
	if(status)
		digitalWrite(this->led_error, 1);
	else
		digitalWrite(this->led_error, 0);

	return;
}