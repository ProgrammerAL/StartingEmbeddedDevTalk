#ifndef AppRunner_H
#define AppRunner_H

#include <stdint.h>
#include <Entities/include/ReceivedData.h>
#include <Utilities/include/IntByteConversionUtilities.h>
#include <App/AppLogic/include/AppSettings.h>
#include <MessageHandling/include/BinarySerializableMessageUtils.h>
#include <MessageHandling/include/Messages/Device/DeviceBroadcastMessage.h>

using namespace OrlandoIotDemo::Entities::Rf;
using namespace OrlandoIotDemo::Device::Messaging;

namespace OrlandoIotDemo
{
	namespace Device
	{
		namespace App
		{
			class AppRunner
			{
			private:
				static const uint8_t InvalidPcuConnectionId = 0;
				static const uint16_t TimeBetweenRfTransmitsInMs = 100;//10 times a second

				AppSettings& _settings;
				IntByteConversionUtilities& _intConversionUtils;
				BinarySerializableMessageUtils _binaryMessageUtils;

				DeviceBroadcastMessage _deviceConnectBroadcastMessage;

				uint32_t _transmitNextRunTime;

				ReceivedData(*_receiveRfMessageFunction)(void);
				void(*_transmitRfMessageFunction)(uint8_t toAddress, const void* buffer, uint8_t bufferSize);
				uint32_t(*_millisFunction)(void);
				void(*_blinkDebugLed)(void);

				void handleReceivedMessage(ReceivedData& receivedData);
				void bradcastMessage();

			public:
				//Constructor
				//	receiveRfMessageFunction - Function that will attempt to receive and RF message
				//	transmitRfMessageFunction - Function that will transmit an RF message
				//	millisFunction - function that will return milliseconds since starting
				//	blinkDebugLed - function that will blink a debug LED
				AppRunner(AppSettings& settings,
					IntByteConversionUtilities& intConversionUtils,
					ReceivedData(*receiveRfMessageFunction)(void),
					void(*transmitRfMessageFunction)(uint8_t toAddress, const void* buffer, uint8_t bufferSize),
					uint32_t(*millisFunction)(void),
					void(*blinkDebugLed)(void)
				);

				void runDeviceFrame();
			};
		}
	}
}

#endif
