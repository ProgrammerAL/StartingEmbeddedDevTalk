#include <Utilities/include/IntByteConversionUtilities.h>

namespace OrlandoIotDemo
{
	namespace Device
	{
		namespace Utilities
		{
			void IntByteConversionUtilities::serializeUInt32ToBytes(const uint32_t paramInt, UnsafeByteArraySlice& outByteValues) const
			{
				//We know there are only 4 bytes in 32 bits, so we don't need a loop
				//Grab the pointer to the 32-bit number and use indexing to grab 1 byte at a time
				uint8_t *valuePointer = (uint8_t*)&paramInt;

				outByteValues.setValue(uint8_t(3), valuePointer[0]);
				outByteValues.setValue(uint8_t(2), valuePointer[1]);
				outByteValues.setValue(uint8_t(1), valuePointer[2]);
				outByteValues.setValue(uint8_t(0), valuePointer[3]);
			}

			void IntByteConversionUtilities::serializeUInt16ToBytes(const uint16_t paramInt, UnsafeByteArraySlice& outByteValues) const
			{
				//Want to use this code so it matches the 32-bit stuff, but it crashes on arduino board. So it shifting it is
				//Grab the pointer to the 16-bit number and use indexing to grab 1 byte at a time
				//uint8_t *valuePointer = (uint8_t*)&paramInt;
				//outByteValues.setValue(uint8_t(1), valuePointer[0]);
				//outByteValues.setValue(uint8_t(0), valuePointer[1]);

				//We know there are only 2 bytes in 16 bits, so we don't need a loop
				//Use bit shifting to only grab the parts of the number we want, then shift over so we only use those bits for the 8 bit number
				uint8_t val = ((paramInt & 0b1111111100000000) >> 8);
				outByteValues.setValue(0, val);

				val = ((paramInt & 0b0000000011111111) >> 0);
				outByteValues.setValue(1, val);
			}

			void IntByteConversionUtilities::serializeUInt8ToBytes(const uint8_t paramInt, UnsafeByteArraySlice& outByteValues) const
			{
				//We know there's only 1 byte in 8 bits, so we don't need a loop
				outByteValues.setValue(uint8_t(0), paramInt);
			}

			uint32_t IntByteConversionUtilities::deserializeBytesToUInt32(const UnsafeByteArraySlice& bytes) const
			{
				uint8_t valueAtIndex_0 = bytes.getValue(0);
				uint8_t valueAtIndex_1 = bytes.getValue(1);
				uint8_t valueAtIndex_2 = bytes.getValue(2);
				uint8_t valueAtIndex_3 = bytes.getValue(3);
				
				return (valueAtIndex_3) | (valueAtIndex_2 << 8) | (valueAtIndex_1 << 16) | (valueAtIndex_0 << 24);
			}

			uint16_t IntByteConversionUtilities::deserializeBytesToUInt16(const UnsafeByteArraySlice& bytes) const
			{
				uint8_t valueAtIndex_0 = bytes.getValue(0);
				uint8_t valueAtIndex_1 = bytes.getValue(1);

				return (valueAtIndex_1) | (valueAtIndex_0 << 8);
			}

			uint8_t IntByteConversionUtilities::deserializeBytesToUInt8(const UnsafeByteArraySlice& bytes) const
			{
				return bytes.getValue(0);
			}
		}
	}
}
