#include "gmock/gmock.h"
#include "device_driver.h"
#include <string>
using std::string;
using namespace testing;
class MockFlashMemoryDevice : public FlashMemoryDevice
{
public:
	MOCK_METHOD(unsigned char, read, (long address), (override));
	MOCK_METHOD(void, write, (long address, unsigned char data), (override));
};

TEST(DeviceDriver, CheckReadCountIsFive) {
	const int READ_ADDRESS = 0xFF;
	NiceMock< MockFlashMemoryDevice> mockHardware;
	EXPECT_CALL(mockHardware, read(READ_ADDRESS))
		.Times(5);

	DeviceDriver driver{ &mockHardware };
	int data = driver.read(READ_ADDRESS);
	EXPECT_EQ(0, data);
}
TEST(DeviceDriver, CheckReadException) {
	const int READ_ADDRESS = 0xFF;
	NiceMock< MockFlashMemoryDevice> mockHardware;
	EXPECT_CALL(mockHardware, read(READ_ADDRESS))
		.Times(5)
		.WillOnce(Return(0))
		.WillOnce(Return(0))
		.WillOnce(Return(0))
		.WillOnce(Return(0))
		.WillOnce(Return(1));

	DeviceDriver driver{ &mockHardware };
	try {
		int data = driver.read(READ_ADDRESS);
		FAIL();
	}
	catch (ReadFailException& exception) {
		EXPECT_EQ(string{ exception.what() }, string{ "Read value is different" });
	}
}



int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}