#ifndef BinarySerializableMessageUtils_H
#define BinarySerializableMessageUtils_H

#include <Utilities/include/IntByteConversionUtilities.h>
#include <MessageHandling/include/MessageType.h>
#include <stdint.h>

using namespace OrlandoIotDemo::Device::Utilities;


namespace OrlandoIotDemo
{
	namespace Device
	{
		namespace Messaging
		{
			class BinarySerializableMessageUtils
			{
			private:
				IntByteConversionUtilities& _conversionUtils;

			public:
				BinarySerializableMessageUtils(IntByteConversionUtilities& conversionUtils);

				MessageType getMessageType(uint8_t* messageBuffer, const uint8_t messageLength) const;
			};
		}
	}
}

#endif