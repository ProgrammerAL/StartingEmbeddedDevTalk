#ifndef DeviceType_H
#define DeviceType_H

#include <stdint.h>

namespace OrlandoIotDemo
{
	namespace Device
	{
		namespace Messaging
		{
			enum DeviceType : uint16_t
			{
				DeviceType_Unknown = 0,
				DeviceType_MyDevice = 1,
			};
		}
	}
}

#endif