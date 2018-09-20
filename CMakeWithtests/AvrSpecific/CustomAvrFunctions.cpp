#ifndef CustomAvrFunctions_Cpp
#define CustomAvrFunctions_Cpp

// For AVR stuff we need to supply our own function to call in case we accidentally call a funtion that
//		never got implemented. Need a better solution than a forever loop. Debugging this will 
//		be a pain. 
// https://stackoverflow.com/questions/920500/what-is-the-purpose-of-cxa-pure-virtual

#include <avr/io.h>
#include <util/delay.h>

void setPinHigh(int pinNum)
{
	PORTD |= (1 << pinNum);
}
void setPinLow(int pinNum)
{
	PORTD &= ~(1 << pinNum);
}

extern "C" void __cxa_pure_virtual()
{
	//TODO: Make something a little better. Just blink an LED a lot right now
	const int ledRegister = DDD3;
	const int ledPortNum = PD3;
	while (1)
	{
		setPinHigh(ledPortNum);
		_delay_ms(100);
		setPinLow(ledPortNum);
		_delay_ms(100);
	}
}

#endif