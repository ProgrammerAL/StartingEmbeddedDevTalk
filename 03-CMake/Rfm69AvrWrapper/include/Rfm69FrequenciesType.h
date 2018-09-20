#ifndef Rfm69FrequenciesType_H
#define Rfm69FrequenciesType_H

#include <stdint.h>
#include <ThirdParty/RFM69-Library-AVR/RFM69registers.h>

namespace OrlandoIotDemo
{
	namespace Device
	{
		namespace Rfm69Avr
		{
			enum Rfm69FrequenciesType : uint16_t
			{
				Rfm69FrequenciesType_Mhz315 = RF_915MHZ,
				Rfm69FrequenciesType_Mhz433 = RF_915MHZ,
				Rfm69FrequenciesType_Mhz868 = RF_868MHZ,
				Rfm69FrequenciesType_Mhz915 = RF_915MHZ,
			};
		}
	}
}

#endif