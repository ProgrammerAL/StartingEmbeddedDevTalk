#include <App/AppLogic/include/AppRunner.h>

using namespace OrlandoIotDemo::Device::Messaging;
using namespace OrlandoIotDemo::Device::Utilities;

namespace OrlandoIotDemo
{
	namespace Device
	{
		namespace App
		{
			AppRunner::AppRunner(AppSettings& settings,
				IntByteConversionUtilities& intConversionUtils,
				ReceivedData(*receiveRfMessageFunction)(void),
				void(*transmitRfMessageFunction)(uint8_t toAddress, const void* buffer, uint8_t bufferSize),
				uint32_t(*millisFunction)(void),
				void(*blinkDebugLed)(void))
				: _settings(settings),
				_receiveRfMessageFunction(receiveRfMessageFunction),
				_transmitRfMessageFunction(transmitRfMessageFunction),
				_millisFunction(millisFunction),
				_blinkDebugLed(blinkDebugLed),
				_intConversionUtils(intConversionUtils),
				_transmitNextRunTime(0),
				_binaryMessageUtils(BinarySerializableMessageUtils(intConversionUtils)),
				_deviceConnectBroadcastMessage(DeviceBroadcastMessage(settings.getDeviceUniqueId(), settings.getDeviceType(), intConversionUtils))
			{
			}

			void AppRunner::runDeviceFrame()
			{
				if (_millisFunction() >= _transmitNextRunTime)
				{
					bradcastMessage();
					_transmitNextRunTime = _millisFunction() + AppRunner::TimeBetweenRfTransmitsInMs;
				}

				//Always try to receive. If it turns out we need a delay, it can be added later
				auto receivedData = _receiveRfMessageFunction();
				if (receivedData.getHasData())
				{
					handleReceivedMessage(receivedData);
				}
			}

			void AppRunner::handleReceivedMessage(ReceivedData& receivedData)
			{
				auto buffer = receivedData.getBuffer();
				auto bufferLength = receivedData.getBufferLength();
				auto messageType = _binaryMessageUtils.getMessageType(buffer, bufferLength);
				//TODO: Do something with the received message other than blink an LED
				_blinkDebugLed();
			}

			void AppRunner::bradcastMessage()
			{
				uint8_t targetNodeId = _settings.getTargetNodeId();
				uint8_t* messageBuffer = _deviceConnectBroadcastMessage.serializeNextMessage();
				uint8_t messageSize = _deviceConnectBroadcastMessage.getMessageSize();

				_transmitRfMessageFunction(targetNodeId, messageBuffer, messageSize);
			}
		}
	}
}

