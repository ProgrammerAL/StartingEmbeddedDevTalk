#include <DeviceUtils/include/AvrTimeUtilFunctions.h>
#include <stdint.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

namespace OrlandoIotDemo
{
	namespace Device
	{
		namespace DeviceSpecificUtils
		{
			namespace TimeFunctions
			{
				volatile uint32_t timer1_millis;

				void millis_init()
				{
					static bool hasInitializedMillis = false;
					if (!hasInitializedMillis)
					{
						// CTC mode, Clock/8
						TCCR1B |= (1 << WGM12) | (1 << CS11);

						// Load the high byte, then the low byte
						// into the output compare
						OCR1AH = (CTC_MATCH_OVERFLOW >> 8);
						OCR1AL = CTC_MATCH_OVERFLOW;
						sei();

						// Enable the compare match interrupt
#if defined (__AVR_ATmega328__) || defined (__AVR_ATmega328P__)
						TIMSK1 |= (1 << OCIE1A);
#elif defined (__AVR_ATmega64__)
						TIMSK |= (1 << OCIE1A);
#endif
					}
				}

				uint32_t millisecondsSinceInit()
				{
					uint32_t millis_return;
					// ensure this cannnot be disrupted
					ATOMIC_BLOCK(ATOMIC_FORCEON)
					{
						millis_return = timer1_millis;
					}
					return millis_return;
				}

				ISR(TIMER1_COMPA_vect)
				{
					timer1_millis++;
				}
			}
		}
	}
}

