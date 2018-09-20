#include <Entities/include/ReceivedData.h>

namespace OrlandoIotDemo
{
	namespace Entities
	{
		namespace Rf
		{
			ReceivedData::ReceivedData(const bool hasData, uint8_t* dataBuffer, const uint8_t dataLength, const int16_t rssi)
				: _hasData(hasData),
				_dataBuffer(dataBuffer),
				_dataLength(dataLength),
				_rssi(rssi)
			{}

			bool ReceivedData::getHasData() const { return _hasData; }
			uint8_t* ReceivedData::getBuffer() { return _dataBuffer; }
			uint8_t ReceivedData::getBufferLength() const { return _dataLength; }
			int16_t ReceivedData::getRssi() const { return _rssi; }
		}
	}
}
