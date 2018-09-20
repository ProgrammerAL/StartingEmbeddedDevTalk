#ifndef ReceivedData_H
#define ReceivedData_H

#include <stdint.h>

namespace OrlandoIotDemo
{
	namespace Entities
	{
		namespace Rf
		{
			class ReceivedData
			{
			private:
				const bool _hasData;
				uint8_t* _dataBuffer;
				const uint8_t _dataLength;
				const int16_t _rssi;

			public:
				ReceivedData(const bool hasData, uint8_t* dataBuffer, const uint8_t dataLength, const int16_t rssi);

				bool getHasData() const;
				uint8_t* getBuffer();
				uint8_t getBufferLength() const;
				int16_t getRssi() const;
			};
		}
	}
}

#endif