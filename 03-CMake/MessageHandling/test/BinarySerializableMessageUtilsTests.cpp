#include <stdint.h>
#include "gtest/gtest.h"

#include <MessageHandling/include/BinarySerializableMessageUtils.h>
#include <Utilities/include/IntByteConversionUtilities.h>

using namespace OrlandoIotDemo::Device::Messaging;
using namespace OrlandoIotDemo::Device::Utilities;

namespace {

	TEST(BinarySerializableMessageUtilsTest, WhenParsingMessageTypeDeviceDiscovery_AssertParsedCorrectly)
	{
		auto intUtilities = IntByteConversionUtilities();
		auto utils = BinarySerializableMessageUtils(intUtilities);

		auto bufferLength = 2;
		uint8_t buffer[] = { 0, MessageType::MessageType_DeviceBroadcastMessage };

		auto parsedMessageType = utils.getMessageType(buffer, bufferLength);
		EXPECT_EQ(MessageType::MessageType_DeviceBroadcastMessage, parsedMessageType);
	}

	TEST(BinarySerializableMessageUtilsTest, WhenParsingMessageTypeDeviceStatus_AssertParsedCorrectly)
	{
		auto intUtilities = IntByteConversionUtilities();
		auto utils = BinarySerializableMessageUtils(intUtilities);

		auto bufferLength = 2;
		uint8_t buffer[] = { 0, MessageType::MessageType_DeviceBroadcastMessage };

		auto parsedMessageType = utils.getMessageType(buffer, bufferLength);
		EXPECT_EQ(MessageType::MessageType_DeviceBroadcastMessage, parsedMessageType);
	}

	TEST(BinarySerializableMessageUtilsTest, WhenInputBufferTooSmall_AssertResponseIsUnknown)
	{
		auto intUtilities = IntByteConversionUtilities();
		auto utils = BinarySerializableMessageUtils(intUtilities);

		auto bufferLength = 1;
		uint8_t buffer[] = { 2 };

		auto parsedMessageType = utils.getMessageType(buffer, bufferLength);
		EXPECT_EQ(MessageType::MessageType_Unknown, parsedMessageType);
	}

	TEST(BinarySerializableMessageUtilsTest, WhenUnknownMessageParsed_AssertResponseIsUnknown)
	{
		auto intUtilities = IntByteConversionUtilities();
		auto utils = BinarySerializableMessageUtils(intUtilities);

		auto bufferLength = 2;
		uint8_t buffer[] = { 255, 20 };

		auto parsedMessageType = utils.getMessageType(buffer, bufferLength);
		EXPECT_EQ(MessageType::MessageType_Unknown, parsedMessageType);
	}
}
