#include <MessageHandling/include/Messages/Device/DeviceBroadcastMessage.h>

namespace OrlandoIotDemo
{
	namespace Device
	{
		namespace Messaging
		{
			DeviceBroadcastMessage::DeviceBroadcastMessage(uint32_t uniqueDeviceId, DeviceType deviceType, IntByteConversionUtilities& conversionUtils)
				: _binaryMessage(BinarySerializableMessageBase(MessageType::MessageType_DeviceBroadcastMessage, DeviceBroadcastMessage::TotalBytesCount, conversionUtils)),
				_uniqueDeviceId(uniqueDeviceId),
				_deviceType(deviceType),
				_conversionUtils(conversionUtils)
			{
				auto messageBytes = _binaryMessage.getMessageBufferReference();

				//Write data out to buffer now since this never changes. Makes the onSerialize() call a no/op
				auto uniqueDeviceIdSlice = UnsafeByteArraySlice(messageBytes, UniqueDeviceIdStartIndex);
				_conversionUtils.serializeUInt32ToBytes(_uniqueDeviceId, uniqueDeviceIdSlice);

				auto deviceTypeSlice = UnsafeByteArraySlice(messageBytes, DeviceTypeStartIndex);
				_conversionUtils.serializeUInt16ToBytes(_deviceType, deviceTypeSlice);
			}

			uint32_t DeviceBroadcastMessage::getDeviceId() const { return _uniqueDeviceId; }
			DeviceType DeviceBroadcastMessage::getDeviceType() const { return _deviceType; }

			uint8_t* DeviceBroadcastMessage::getLastSerializedMessage()
			{
				return _binaryMessage.getMessageBufferReference();
			}

			uint8_t DeviceBroadcastMessage::getMessageSize() const
			{
				return _binaryMessage.getMessageSize();
			}

			MessageType DeviceBroadcastMessage::getMessageType() const { return MessageType::MessageType_DeviceBroadcastMessage; }

			uint8_t* DeviceBroadcastMessage::serializeNextMessage()
			{
				_binaryMessage.incrementMessageId();
				return _binaryMessage.getMessageBufferReference();
			}
		}
	}
}