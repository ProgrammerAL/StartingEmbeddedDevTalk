#include <Rfm69AvrWrapper/include/Rfm69AvrController.h>

#include <ThirdParty/RFM69-Library-AVR/RFM69.h>
#include <ThirdParty/RFM69-Library-AVR/RFM69.c>

namespace OrlandoIotDemo
{
	namespace Device
	{
		namespace Rfm69Avr
		{
			Rfm69AvrController::Rfm69AvrController()
			{
				//Not needed. Array is initialized in header file
				//for (int i = 0; i < MaxMessageBufferSize; i++)
				//{
				//	_dataBuffer[i] = 0;
				//}
			}

			void Rfm69AvrController::initialize(uint16_t frequency, uint8_t nodeId, uint8_t networkId, bool useHighPower, uint32_t(*millisFunction)(void))
			{
				rfm69_init(frequency, nodeId, networkId, millisFunction);
				setHighPower(useHighPower);
				encrypt(null); //Default to no encryption. if set it has to be 16 bytes. example: "1234567890123456"
			}

			void Rfm69AvrController::setEncryptionKey(const char* key) const
			{
				encrypt(key);
			}

			void Rfm69AvrController::sendMessage(uint8_t toAddress, const void* buffer, uint8_t bufferSize, bool requestACK) const
			{
				send(toAddress, buffer, bufferSize, requestACK);
			}

			ReceivedData Rfm69AvrController::tryReceiveMessage()
			{
				if (receiveDone())
				{
					for (int i = 0; i < MaxMessageBufferSize; i++)
					{
						_dataBuffer[i] = DATA[i];
					}
					ReceivedData data(true, _dataBuffer, DATALEN, readRSSI(false));
					return data;
				}
				else
				{
					ReceivedData data(false, nullptr, 0, 0);
					return data;
				}
			}
		}
	}
}