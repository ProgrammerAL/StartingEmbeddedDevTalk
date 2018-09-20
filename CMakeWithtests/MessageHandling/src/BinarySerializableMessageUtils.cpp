#include <MessageHandling/include/BinarySerializableMessageUtils.h>
#include <MessageHandling/include/BinarySerializableMessageBase.h>

namespace OrlandoIotDemo
{
	namespace Device
	{
		namespace Messaging
		{
			BinarySerializableMessageUtils::BinarySerializableMessageUtils(IntByteConversionUtilities& conversionUtils)
				: _conversionUtils(conversionUtils)
			{
			}

			//TODO: Add validation to make sure the size of the message is the expected size. Otherwise return unknown
			MessageType BinarySerializableMessageUtils::getMessageType(uint8_t* messageBuffer, const uint8_t messageLength) const
			{
				// MessageType of type uint16_t. Make sure the buffer is at least as big as to the end of that field
				//	ie: To where the field starts in the buffer plus the size of MessageType
				static const uint8_t minimumSize = BinarySerializableMessageBase::MessageTypeIndex + sizeof(uint16_t);
				if (messageLength < minimumSize)
				{
					//TODO: Log Error
					return MessageType::MessageType_Unknown;
				}

				UnsafeByteArraySlice messageSlice(messageBuffer, 0);
				auto messageTypeInt = _conversionUtils.deserializeBytesToUInt16(messageSlice);
				if (messageTypeInt > MessageType::MessageType_MaxValue)
				{
					return MessageType::MessageType_Unknown;
				}

				return (MessageType)messageTypeInt;
			}
		}
	}
}
