#ifndef IntByteConversionUtilities_H
#define IntByteConversionUtilities_H

#include <stdint.h>
#include <Utilities/include/UnsafeByteArraySlice.h>

namespace OrlandoIotDemo
{
	namespace Device
	{
		namespace Utilities
		{
			//Note: All serialization.deserialization uses big endianness (bigger numbers on left). 
			//		So everything interacting with this needs to use the same format. Here in the embedded world, and the other apps in the .NET world
			class IntByteConversionUtilities
			{
			public:
				void serializeUInt32ToBytes(const uint32_t paramInt, UnsafeByteArraySlice& outByteValues) const;
				void serializeUInt16ToBytes(const uint16_t paramInt, UnsafeByteArraySlice& outByteValues) const;
				void serializeUInt8ToBytes(const uint8_t paramInt, UnsafeByteArraySlice& outByteValues) const;

				uint32_t deserializeBytesToUInt32(const UnsafeByteArraySlice& bytes) const;
				uint16_t deserializeBytesToUInt16(const UnsafeByteArraySlice& bytes) const;
				uint8_t deserializeBytesToUInt8(const UnsafeByteArraySlice& bytes) const;
			};
		}
	}
}

#endif
