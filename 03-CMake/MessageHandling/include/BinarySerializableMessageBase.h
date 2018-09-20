#ifndef BinarySerializableMessageBase_H
#define BinarySerializableMessageBase_H

#include <stdint.h>

#include "MessageType.h"
#include <Utilities/include/IntByteConversionUtilities.h>
#include <Utilities/include/UnsafeByteArraySlice.h>

using namespace OrlandoIotDemo::Device::Utilities;

namespace OrlandoIotDemo
{
	namespace Device
	{
		namespace Messaging
		{
			class BinarySerializableMessageBase
			{
			public:
				static const uint8_t MaxBufferSize = 61;
				static const uint16_t MessageTypeIndex = 0; // 1st Field always at index 0
				static const uint16_t MessageIdIndex = sizeof(uint16_t);// 2nd Field. Value = 2

			private:
				const uint8_t _messageBufferSize;
				uint8_t _messageArray[MaxBufferSize] = { // Buffer with 61 items
					0,0,0,0,0,0,0,0,0,0,
					0,0,0,0,0,0,0,0,0,0,
					0,0,0,0,0,0,0,0,0,0,
					0,0,0,0,0,0,0,0,0,0,
					0,0,0,0,0,0,0,0,0,0,
					0,0,0,0,0,0,0,0,0,0,
					0
				};

				IntByteConversionUtilities& _conversionUtils;

				UnsafeByteArraySlice _messageIdSlice;
				uint16_t _messageId;

			public:
				// Let anyone inheriting know the array index they can start using for the message in the array when serializing. 
				//	Index of last field + size of the last field = How many bits we're using for this message.
				//	Value = 4
				static const uint16_t BaseMessageLastIndex = MessageIdIndex + sizeof(uint16_t);

				BinarySerializableMessageBase(MessageType messageType, uint8_t bufferSize, IntByteConversionUtilities& conversionUtils);
				BinarySerializableMessageBase(uint8_t* messageArray, uint8_t bufferSize, IntByteConversionUtilities& conversionUtils);
				
				uint8_t getMessageSize() const;
				uint8_t* getMessageBufferReference();
				uint16_t getMessageId() const;

				void incrementMessageId();
			};
		}
	}
}

#endif