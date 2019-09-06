#include "Button.h"
#include <wiringPi.h>

Button::Button(int Pin_)
{
     wiringPiSetup ();
     this->Pin = Pin_;
     pinMode(this->Pin, INPUT);
}

bool Button::isPressed(void)
{
    if(digitalRead(this->Pin) == 0) return 1;
    else return 0;
}
