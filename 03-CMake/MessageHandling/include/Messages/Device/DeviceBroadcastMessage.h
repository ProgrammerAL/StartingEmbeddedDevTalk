#ifndef DeviceBroadcastMessage_H
#define DeviceBroadcastMessage_H

#include <MessageHandling/include/BinarySerializableMessageBase.h>
#include <MessageHandling/include/DeviceType.h>
using namespace OrlandoIotDemo::Device::Messaging;

namespace OrlandoIotDemo
{
	namespace Device
	{
		namespace Messaging
		{
			class DeviceBroadcastMessage
			{
			private:
				static const uint16_t UniqueDeviceIdStartIndex = BinarySerializableMessageBase::BaseMessageLastIndex; // 1st Field from this class. Value == 4
				static const uint16_t DeviceTypeStartIndex = UniqueDeviceIdStartIndex + sizeof(uint32_t);// 2nd Field.  Value == 8

				// Start index of last field + size of the last field = How many bits we're using for this message.
				//	This is a 1-based total count.
				//	Value == 10
				static const uint8_t TotalBytesCount = DeviceTypeStartIndex + sizeof(DeviceType);

				IntByteConversionUtilities& _conversionUtils;
				uint32_t _uniqueDeviceId;
				DeviceType _deviceType;
				BinarySerializableMessageBase _binaryMessage;

			public:
				DeviceBroadcastMessage(uint32_t uniqueDeviceId, DeviceType deviceType, IntByteConversionUtilities& conversionUtils);

				uint32_t getDeviceId() const;
				DeviceType getDeviceType() const;
				MessageType getMessageType() const;
				uint8_t getMessageSize() const;
				uint8_t* getLastSerializedMessage();

				//Ensures the message buffer is serialized and that the Message Id is incremented
				uint8_t* serializeNextMessage();
			};
		}
	}
}

#endif