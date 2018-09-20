#ifndef UnsafeByteArraySlice_H
#define UnsafeByteArraySlice_H

#include <stdint.h>

namespace OrlandoIotDemo
{
	namespace Device
	{
		namespace Utilities
		{
			class UnsafeByteArraySlice
			{

			private:
				uint8_t* _tArray;
				const uint8_t _startingIndex;

				uint8_t getIndexInArray(const uint8_t sliceIndex) const;

			public:
				UnsafeByteArraySlice(uint8_t *tArray, const uint8_t startingIndex);

				uint8_t getValue(const uint8_t sliceIndex) const;
				void setValue(const uint8_t sliceIndex, const uint8_t value);
			};
		}
	}
}

#endif
