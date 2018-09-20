#ifndef TransceiverWrapper_H
#define TransceiverWrapper_H

#include <stdint.h>
#include <Entities/include/ReceivedData.h>

using namespace OrlandoIotDemo::Entities::Rf;

namespace OrlandoIotDemo
{
	namespace Device
	{
		namespace DeviceSpecificUtils
		{
			class TransceiverWrapper
			{
			private:
				ReceivedData(*_receiveTask)(void);
				void(*_transmitTask)(uint8_t toAddress, const void* buffer, uint8_t bufferSize, bool requestACK);

			public:
				TransceiverWrapper(ReceivedData(*receiveTask)(void), void(*transmitTask)(uint8_t toAddress, const void* buffer, uint8_t bufferSize, bool requestACK));

				void transmit(uint8_t toAddress, const void* buffer, uint8_t bufferSize, bool requestACK);
				ReceivedData receive();
			};
		}
	}
}

#endif