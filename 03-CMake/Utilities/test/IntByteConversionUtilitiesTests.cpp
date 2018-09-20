#include <stdint.h>
#include "gtest/gtest.h"

#include <Utilities/include/IntByteConversionUtilities.h>
#include <Utilities/include/UnsafeByteArraySlice.h>

using namespace OrlandoIotDemo::Device::Utilities;

namespace {

	TEST(IntByteConversionUtilitiesTests_UInt32Conversion, ExpectUInt32ConvertedToArray_SmallNumber) {
		auto intBytes = new uint8_t[4];
		auto slice = UnsafeByteArraySlice(intBytes, 0);
		auto num = uint32_t(10);
		
		auto util = IntByteConversionUtilities();
		util.serializeUInt32ToBytes(num, slice);
		
		uint8_t valueAtIndex_0 = slice.getValue(0);
		uint8_t valueAtIndex_1 = slice.getValue(1);
		uint8_t valueAtIndex_2 = slice.getValue(2);
		uint8_t valueAtIndex_3 = slice.getValue(3);

		EXPECT_EQ(0, valueAtIndex_0);
		EXPECT_EQ(0, valueAtIndex_1);
		EXPECT_EQ(0, valueAtIndex_2);
		EXPECT_EQ(10, valueAtIndex_3);
	}

	TEST(IntByteConversionUtilitiesTests_UInt32Conversion, ExpectUInt32ConvertedToArray_LargeNumber) {
		auto intBytes = new uint8_t[4];
		auto slice = UnsafeByteArraySlice(intBytes, 0);

		auto num = uint32_t(0xffffffff) - 3; // 1 less than max uint32 value

		auto util = IntByteConversionUtilities();
		util.serializeUInt32ToBytes(num, slice);

		uint8_t valueAtIndex_0 = slice.getValue(0);
		uint8_t valueAtIndex_1 = slice.getValue(1);
		uint8_t valueAtIndex_2 = slice.getValue(2);
		uint8_t valueAtIndex_3 = slice.getValue(3);

		EXPECT_EQ(255, valueAtIndex_0);
		EXPECT_EQ(255, valueAtIndex_1);
		EXPECT_EQ(255, valueAtIndex_2);
		EXPECT_EQ(252, valueAtIndex_3);
	}

	TEST(IntByteConversionUtilitiesTests_UInt16Conversion, ExpectUInt16ConvertedToArray_SmallNumber) {
		auto intBytes = new uint8_t[2];
		auto num = uint16_t(10);
		auto slice = UnsafeByteArraySlice(intBytes, 0);

		auto util = IntByteConversionUtilities();
		util.serializeUInt16ToBytes(num, slice);

		uint8_t valueAtIndex_0 = slice.getValue(0);
		uint8_t valueAtIndex_1 = slice.getValue(1);

		EXPECT_EQ(0, valueAtIndex_0);
		EXPECT_EQ(10, valueAtIndex_1);
	}

	TEST(IntByteConversionUtilitiesTests_UInt16Conversion, ExpectUInt16ConvertedToArray_LargeNumber) {
		auto intByes = new uint8_t[2];
		auto slice = UnsafeByteArraySlice(intByes, 0);

		auto num = uint16_t(0xffff) - 3; // 1 less than max uint16 value

		auto util = IntByteConversionUtilities();
		util.serializeUInt16ToBytes(num, slice);

		uint8_t valueAtIndex_0 = slice.getValue(0);
		uint8_t valueAtIndex_1 = slice.getValue(1);

		EXPECT_EQ(255, valueAtIndex_0);
		EXPECT_EQ(252, valueAtIndex_1);
	}

	TEST(IntByteConversionUtilitiesTests_UInt8Conversion, ExpectUInt8ConvertedToArray_SmallNumber) {
		auto intBytes = new uint8_t[1];
		auto num = uint8_t(10);
		auto slice = UnsafeByteArraySlice(intBytes, 0);

		auto util = IntByteConversionUtilities();
		util.serializeUInt8ToBytes(num, slice);

		uint8_t valueAtIndex_0 = slice.getValue(0);
		EXPECT_EQ(10, valueAtIndex_0);
	}

	TEST(IntByteConversionUtilitiesTests_UInt8Conversion, ExpectUInt8ConvertedToArray_LargeNumber) {
		auto intBytes = new uint8_t[1];
		auto slice = UnsafeByteArraySlice(intBytes, 0);

		auto num = uint8_t(0xff) - 1;// 1 less than max uint8 value

		auto util = IntByteConversionUtilities();
		util.serializeUInt8ToBytes(num, slice);

		uint8_t valueAtIndex_0 = slice.getValue(0);
		EXPECT_EQ(254, valueAtIndex_0);
	}

	TEST(IntByteConversionUtilitiesTests_ConvertToUInt32, ExpectArrayConvertedToUInt32) {
		auto intBytes = new uint8_t[4]{255, 255, 255, 254};
		auto expectedValue = uint32_t(0xffffffff) - 1; // 1 less than max uint32 value
		auto slice = UnsafeByteArraySlice(intBytes, 0);

		auto util = IntByteConversionUtilities();
		auto result = util.deserializeBytesToUInt32(slice);

		EXPECT_EQ(expectedValue, result);
	}

	TEST(IntByteConversionUtilitiesTests_ConvertToUInt16, ExpectArrayConvertedToUInt16) {
		auto intBytes = new uint8_t[2]{ 255, 254 };
		auto expectedValue = uint16_t(0xffff) - 1; // 1 less than max uint16 value
		auto slice = UnsafeByteArraySlice(intBytes, 0);

		auto util = IntByteConversionUtilities();
		auto result = util.deserializeBytesToUInt16(slice);

		EXPECT_EQ(expectedValue, result);
	}

	TEST(IntByteConversionUtilitiesTests_ConvertToUInt8, ExpectArrayConvertedToUInt8) {
		auto intBytes = new uint8_t[1]{ 254 };
		auto expectedValue = uint8_t(0xff) - 1; // 1 less than max uint8 value
		auto slice = UnsafeByteArraySlice(intBytes, 0);

		auto util = IntByteConversionUtilities();
		auto result = util.deserializeBytesToUInt8(slice);

		EXPECT_EQ(expectedValue, result);
	}
}
