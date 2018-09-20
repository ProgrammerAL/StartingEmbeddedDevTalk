#include <DeviceUtils/include/TransceiverUtils.h>
#include <DeviceUtils/include/AvrTimeUtilFunctions.h>

namespace OrlandoIotDemo
{
	namespace Device
	{
		namespace DeviceSpecificUtils
		{
			void TransceiverUtils::initTransceiver(Rfm69AvrController& outTransceiver, Rfm69FrequenciesType frequency, uint8_t nodeId, uint8_t networkId, bool useHighPower)
			{
				outTransceiver.initialize(915, nodeId, networkId, useHighPower, &(OrlandoIotDemo::Device::DeviceSpecificUtils::TimeFunctions::millisecondsSinceInit));
			}
		}
	}
}

