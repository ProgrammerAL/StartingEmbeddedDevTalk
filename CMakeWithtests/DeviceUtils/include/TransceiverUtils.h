#ifndef TransceiverUtils_H
#define TransceiverUtils_H

#include <stdint.h>
#include <Rfm69AvrWrapper/include/Rfm69AvrController.h>
#include <Rfm69AvrWrapper/include/Rfm69FrequenciesType.h>

using namespace OrlandoIotDemo::Device::Rfm69Avr;

namespace OrlandoIotDemo
{
	namespace Device
	{
		namespace DeviceSpecificUtils
		{
			class TransceiverUtils
			{
			public:
				inline TransceiverUtils() {};

				void initTransceiver(Rfm69AvrController& outTransceiver, Rfm69FrequenciesType frequency, uint8_t nodeId, uint8_t networkId, bool useHighPower);
			};
		}
	}
}

#endif