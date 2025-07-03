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
private:
	int data;
};

class DeviceDriverFixture : public Test {
protected:
	void SetUp() override {
	}
public:
	NiceMock< MockFlashMemoryDevice> mockHardware;
	DeviceDriver driver{ &mockHardware };
};
TEST_F(DeviceDriverFixture, CheckReadCountIsFive) {
	EXPECT_CALL(mockHardware, read)
		.Times(5);

	int data = driver.read(0xA);
	EXPECT_EQ(0, data);
}
TEST_F(DeviceDriverFixture, CheckReadException) {
	EXPECT_CALL(mockHardware, read)
		.Times(5)
		.WillOnce(Return(0))
		.WillOnce(Return(0))
		.WillOnce(Return(0))
		.WillOnce(Return(0))
		.WillOnce(Return(1));

	try {
		int data = driver.read(0xA);
		FAIL();
	}
	catch (ReadFailException& exception) {
		EXPECT_EQ(string{ exception.what() }, string{ "Read value is different" });
	}
}

TEST_F(DeviceDriverFixture, CheckReadException2) {
	EXPECT_CALL(mockHardware, read)
		.Times(2)
		.WillOnce(Return(0))
		.WillOnce(Return(1));

	try {
		int data = driver.read(0xA);
		FAIL();
	}
	catch (ReadFailException& exception) {
		EXPECT_EQ(string{ exception.what() }, string{ "Read value is different" });
	}
}

TEST_F(DeviceDriverFixture, CheckWriteDone) {
	const int address = 0xB;
	EXPECT_CALL(mockHardware, read(address))
		.WillOnce(Return(0xFF))
		.WillRepeatedly(Return(0x11));

	EXPECT_CALL(mockHardware, write(address, 0x11))
		.WillOnce(Return());

	driver.write(address, 0x11);
	auto data = driver.read(address);
	EXPECT_EQ(0x11, data);
}

TEST_F(DeviceDriverFixture, CheckWriteFailException) {
	const int address = 0xC;
	EXPECT_CALL(mockHardware, read(address))
		.WillOnce(Return(0x0));

	try {
		driver.write(address, 0x11);
		FAIL();
	}
	catch (WriteFailException& exception) {
		EXPECT_EQ(string{ exception.what() }, string{ "Already written" });
	}
}
int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}