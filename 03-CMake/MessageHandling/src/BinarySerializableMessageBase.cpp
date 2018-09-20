#include <MessageHandling/include/BinarySerializableMessageBase.h>

namespace OrlandoIotDemo
{
	namespace Device
	{
		namespace Messaging
		{
			BinarySerializableMessageBase::BinarySerializableMessageBase(MessageType messageType, uint8_t bufferSize, IntByteConversionUtilities& conversionUtils)
				:
				_conversionUtils(conversionUtils),
				_messageId(0),
				_messageBufferSize(bufferSize),
				_messageIdSlice(UnsafeByteArraySlice(_messageArray, MessageIdIndex))
			{
				// Write this to buffer now because it never changes
				auto messageTypeSlice = UnsafeByteArraySlice(_messageArray, MessageTypeIndex);
				_conversionUtils.serializeUInt16ToBytes(messageType, messageTypeSlice);
			}

			BinarySerializableMessageBase::BinarySerializableMessageBase(uint8_t* messageArray, uint8_t bufferSize, IntByteConversionUtilities& conversionUtils)
				:
				_conversionUtils(conversionUtils),
				_messageBufferSize(bufferSize),
				_messageIdSlice(UnsafeByteArraySlice(_messageArray, MessageIdIndex))
			{
				//Since we can't use memcpy(), copy bytes over by hand
				for (uint8_t i = 0; i < bufferSize; i++)
				{
					_messageArray[i] = messageArray[i];
				}

				_messageId = _conversionUtils.deserializeBytesToUInt16(_messageIdSlice);
			}

			//BinarySerializableMessageBase::~BinarySerializableMessageBase()
			//{
			//	TODO: Find out if this is needed
			//	delete _messageArray;
			//}

			uint8_t BinarySerializableMessageBase::getMessageSize() const
			{
				return _messageBufferSize;
			}

			uint8_t* BinarySerializableMessageBase::getMessageBufferReference()
			{
				return _messageArray;
			}

			void BinarySerializableMessageBase::incrementMessageId()
			{
				_messageId++;
				_conversionUtils.serializeUInt16ToBytes(_messageId, _messageIdSlice);
			}

			uint16_t BinarySerializableMessageBase::getMessageId() const
			{
				return _messageId;
			}
		}
	}
}

