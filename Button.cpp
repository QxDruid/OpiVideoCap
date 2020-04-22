#include "Button.h"
#include <wiringPi.h>

Button::Button(int Pin_)
{
    this->Pin = Pin_;
}

bool Button::isPressed(void)
{
    if(digitalRead(this->Pin) == 0) return 1;
    else return 0;
}

void Button::init()
{
	
    pinMode(this->Pin, INPUT);	
}
