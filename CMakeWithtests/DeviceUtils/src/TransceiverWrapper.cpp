#include <DeviceUtils/include/TransceiverWrapper.h>

namespace OrlandoIotDemo
{
	namespace Device
	{
		namespace DeviceSpecificUtils
		{
			TransceiverWrapper::TransceiverWrapper(ReceivedData(*receiveTask)(void), void(*transmitTask)(uint8_t toAddress, const void* buffer, uint8_t bufferSize, bool requestACK))
				:_receiveTask(receiveTask),
				_transmitTask(transmitTask)
			{
			}

			void TransceiverWrapper::transmit(uint8_t toAddress, const void* buffer, uint8_t bufferSize, bool requestACK)
			{
				_transmitTask(toAddress, buffer, bufferSize, requestACK);
			}

			ReceivedData TransceiverWrapper::receive()
			{
				return _receiveTask();
			}
		}
	}
}
