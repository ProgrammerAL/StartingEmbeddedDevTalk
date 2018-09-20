#include <stdint.h>
#include "gtest/gtest.h"

#include <MessageHandling/include/Messages/Device/DeviceBroadcastMessage.h>
#include <MessageHandling/include/DeviceType.h>

#include <Utilities/include/UnsafeByteArraySlice.h>
#include <Utilities/include/IntByteConversionUtilities.h>

using namespace OrlandoIotDemo::Device::Messaging;
using namespace OrlandoIotDemo::Device::Utilities;

namespace {

	TEST(DeviceBroadcastMessageTests, ExpectSerializationOutput)
	{
		auto utilities = IntByteConversionUtilities();

		auto message = DeviceBroadcastMessage(4294967295, DeviceType::DeviceType_MyDevice, utilities);

		auto messageSize = message.getMessageSize();
		EXPECT_EQ(10, messageSize);

		auto serializedMessage = message.serializeNextMessage();
		serializedMessage = message.serializeNextMessage();
		serializedMessage = message.serializeNextMessage();

		auto messageTypeSlice = UnsafeByteArraySlice(serializedMessage, 0);
		auto messageIdSlice = UnsafeByteArraySlice(serializedMessage, 2);
		auto deviceIdSlice = UnsafeByteArraySlice(serializedMessage, 4);
		auto deviceTypeSlice = UnsafeByteArraySlice(serializedMessage, 8);

		auto messageType = utilities.deserializeBytesToUInt16(messageTypeSlice);
		auto messageId = utilities.deserializeBytesToUInt16(messageIdSlice);
		auto deviceId = utilities.deserializeBytesToUInt32(deviceIdSlice);
		auto deviceType = utilities.deserializeBytesToUInt16(deviceTypeSlice);

		EXPECT_EQ(MessageType::MessageType_DeviceBroadcastMessage, messageType);
		EXPECT_EQ(3, messageId);
		EXPECT_EQ(4294967295, deviceId);
		EXPECT_EQ(DeviceType::DeviceType_MyDevice, deviceType);
	}
}
