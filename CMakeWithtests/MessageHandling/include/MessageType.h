#ifndef MessageType_H
#define MessageType_H

#include <stdint.h>

namespace OrlandoIotDemo
{
	namespace Device
	{
		namespace Messaging
		{
			enum MessageType : uint16_t
			{
				MessageType_Unknown = 0,
				MessageType_DeviceBroadcastMessage = 1,

				MessageType_MaxValue = MessageType_DeviceBroadcastMessage
			};
		}
	}
}

#endif
