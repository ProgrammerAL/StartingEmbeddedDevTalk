#include <AvrSpecific/CustomAvrFunctions.cpp> //This needs to be first. Sets up some constants used by AVR stuff

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>

#include <Rfm69AvrWrapper/include/Rfm69AvrController.h>
#include <DeviceUtils/include/TransceiverUtils.h>
#include <DeviceUtils/include/AvrTimeUtilFunctions.h>
#include <App/AppLogic/include/AppRunner.h>
#include <App/AppLogic/include/AppSettings.h>

using namespace OrlandoIotDemo::Device::App;
using namespace OrlandoIotDemo::Device::DeviceSpecificUtils;
using namespace OrlandoIotDemo::Device::DeviceSpecificUtils::TimeFunctions;
using namespace OrlandoIotDemo::Device::Rfm69Avr;


void setPinToBeOutputPin(int bRegister)
{
	DDRD |= (1 << bRegister);
}
void setPinToBeInputPin(int bRegister)
{
	DDRD &= ~(1 << bRegister);
}
void setPinOputputToHigh(int pinNum)
{
	PORTD |= (1 << pinNum);
}
void setPinOputputToLow(int pinNum)
{
	PORTD &= ~(1 << pinNum);
}

const int ledRegister = DDD3;
const int ledPortNum = PD3;

Rfm69AvrController _transceiver;

ReceivedData receiveRfMessageFunction(void)
{
	return _transceiver.tryReceiveMessage();
}

void transmitRfMessageFunction(uint8_t toAddress, const void* buffer, uint8_t bufferSize)
{
	return _transceiver.sendMessage(toAddress, buffer, bufferSize, false);
}

void blinkDebugLed()
{
	setPinOputputToHigh(ledPortNum);
	_delay_ms(100);
	setPinOputputToLow(ledPortNum);
}

int main()
{
	setPinToBeOutputPin(DDD3);
	millis_init();

	AppSettings settings(100, 2, 1, 1, DeviceType::DeviceType_MyDevice);
	IntByteConversionUtilities intConversionUtils;

	TransceiverUtils transceiverUtils;
	transceiverUtils.initTransceiver(_transceiver, Rfm69FrequenciesType::Rfm69FrequenciesType_Mhz915, settings.getNodeId(), settings.getNetworkId(), true);

	uint8_t timeSinceLastBroadcastInMs = 0;

	AppRunner runner(settings, intConversionUtils, &receiveRfMessageFunction, &transmitRfMessageFunction, &(OrlandoIotDemo::Device::DeviceSpecificUtils::TimeFunctions::millisecondsSinceInit), &(blinkDebugLed));

	while (true)
	{
		runner.runDeviceFrame();
	}

	//LED Debug function
	//uint16_t delayTime = 500;
	//while (1)
	//{
	//	setPinOputputToHigh(ledPortNum);
	//	_delay_ms(delayTime);
	//	setPinOputputToLow(ledPortNum);
	//	_delay_ms(delayTime);
	//}

	return 0;
}


