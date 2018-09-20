#include <Utilities/include/UnsafeByteArraySlice.h>

namespace OrlandoIotDemo
{
	namespace Device
	{
		namespace Utilities
		{
			UnsafeByteArraySlice::UnsafeByteArraySlice(uint8_t *tArray, const uint8_t startingIndex)
				: _tArray(tArray),
				_startingIndex(startingIndex)
			{
			}

			uint8_t UnsafeByteArraySlice::getIndexInArray(const uint8_t sliceIndex) const
			{
				auto indexInCollection = sliceIndex + this->_startingIndex;
				return indexInCollection;
			}

			uint8_t UnsafeByteArraySlice::getValue(const uint8_t sliceIndex) const
			{
				auto indexInCollection = getIndexInArray(sliceIndex);
				return _tArray[indexInCollection];
			}

			void UnsafeByteArraySlice::setValue(const uint8_t sliceIndex, const uint8_t value)
			{
				auto indexInCollection = getIndexInArray(sliceIndex);
				_tArray[indexInCollection] = value;
			}
		}
	}
}
