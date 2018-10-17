/*
 * AtemlStudioGccApplication.cpp
 *
 * Created: 10/17/2018 2:31:48 PM
 * Author : Progr
 */ 

#include <avr/io.h>
#include <util/delay.h>


void setPinToBeOutputPin(int bRegister)
{
	DDRD |= (1 << bRegister);
}
void setPinToBeInputPin(int bRegister)
{
	DDRD &= ~(1 << bRegister);
}
void setPinOputputToHigh(int pinNum)
{
	PORTD |= (1 << pinNum);
}
void setPinOputputToLow(int pinNum)
{
	PORTD &= ~(1 << pinNum);
}

const int ledRegister = DDD3;
const int ledPortNum = PD3;


int main()
{
	setPinToBeOutputPin(ledRegister);

	//LED Debug function
	uint16_t delayTime = 500;
	while (1)
	{
		setPinOputputToHigh(ledPortNum);
		_delay_ms(delayTime);
		setPinOputputToLow(ledPortNum);
		_delay_ms(delayTime);
	}

	return 0;
}



