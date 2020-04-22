#ifndef __LED__
#define __LED__

#define ON 1
#define OFF 0

#include <wiringPi.h>

class Led
{
public:
	Led();
	~Led();
	void init();
	void Ok(bool status);
	void Error(bool status);
	void Off();

private:
	const int led_ok = 9;
	const int led_error = 7;
	
};






#endif