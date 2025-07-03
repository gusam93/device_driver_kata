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


class DeviceDriverFixture : public Test {
protected:
	void SetUp() override {

	}
public:
	NiceMock< MockFlashMemoryDevice> mockHardware;
	DeviceDriver driver{ &mockHardware };
	const int READ_ADDRESS = 0xFF;
};
TEST_F(DeviceDriverFixture, CheckReadCountIsFive) {
	EXPECT_CALL(mockHardware, read(READ_ADDRESS))
		.Times(5);

	int data = driver.read(READ_ADDRESS);
	EXPECT_EQ(0, data);
}
TEST_F(DeviceDriverFixture, CheckReadException) {
	EXPECT_CALL(mockHardware, read(READ_ADDRESS))
		.Times(5)
		.WillOnce(Return(0))
		.WillOnce(Return(0))
		.WillOnce(Return(0))
		.WillOnce(Return(0))
		.WillOnce(Return(1));

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