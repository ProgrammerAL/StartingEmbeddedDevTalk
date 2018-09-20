#ifndef AppSettings_H
#define AppSettings_H

#include <MessageHandling/include/DeviceType.h>
#include <stdint.h>

using namespace OrlandoIotDemo::Device::Messaging;

namespace OrlandoIotDemo
{
	namespace Device
	{
		namespace App
		{
			class AppSettings
			{
			private:
				const uint8_t _networkId;
				const uint8_t _myNodeId;
				const uint8_t _targetNodeId;
				const uint32_t _deviceUniqueId;
				const DeviceType _deviceType;

			public:
				AppSettings(const uint8_t networkId,
					const uint8_t myNodeId,
					const uint8_t targetNodeId,
					const uint32_t deviceUniqueId,
					const DeviceType deviceType
				);

				uint8_t getNetworkId() const;
				uint8_t getNodeId() const;
				uint8_t getTargetNodeId() const;
				uint32_t getDeviceUniqueId() const;
				DeviceType getDeviceType() const;
			};
		}
	}
}

#endif
