#include <App/AppLogic/include/AppSettings.h>

namespace OrlandoIotDemo
{
	namespace Device
	{
		namespace App
		{
			AppSettings::AppSettings(
				const uint8_t networkId,
				const uint8_t myNodeId,
				const uint8_t targetNodeId,
				const uint32_t deviceUniqueId,
				const DeviceType deviceType
			)
				: _networkId(networkId),
				_myNodeId(myNodeId),
				_targetNodeId(targetNodeId),
				_deviceUniqueId(deviceUniqueId),
				_deviceType(deviceType)
			{
			}

			uint8_t AppSettings::getNetworkId() const { return _networkId; }
			uint8_t AppSettings::getNodeId() const { return _myNodeId; }
			uint8_t AppSettings::getTargetNodeId() const { return _targetNodeId; }
			uint32_t AppSettings::getDeviceUniqueId() const { return _deviceUniqueId; }
			DeviceType AppSettings::getDeviceType() const { return _deviceType; }
		}
	}
}

