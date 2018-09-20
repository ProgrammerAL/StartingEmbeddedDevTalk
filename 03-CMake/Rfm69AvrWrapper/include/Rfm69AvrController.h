#ifndef Rfm69AvrController_H
#define Rfm69AvrController_H

#include <Entities/include/ReceivedData.h>
using namespace OrlandoIotDemo::Entities::Rf;

namespace OrlandoIotDemo
{
	namespace Device
	{
		namespace Rfm69Avr
		{
			class Rfm69AvrController
			{
			private:
				static const uint8_t MaxMessageBufferSize = 61;
				uint8_t _dataBuffer[MaxMessageBufferSize] = { // Create buffer with 61 items
					0,0,0,0,0,0,0,0,0,0,
					0,0,0,0,0,0,0,0,0,0,
					0,0,0,0,0,0,0,0,0,0,
					0,0,0,0,0,0,0,0,0,0,
					0,0,0,0,0,0,0,0,0,0,
					0,0,0,0,0,0,0,0,0,0,
					0
				};


			public:
				Rfm69AvrController();
				void initialize(uint16_t frequency, uint8_t nodeId, uint8_t networkId, bool useHighPower, uint32_t(*millisFunction)(void));
				void sendMessage(uint8_t toAddress, const void* buffer, uint8_t bufferSize, bool requestACK) const;
				ReceivedData tryReceiveMessage();

				//If set it has to be 16 bytes. example: "1234567890123456"
				void setEncryptionKey(const char* key) const;
			};
		}
	}
}

#endif