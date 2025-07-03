#include "gmock/gmock.h"
#include "device_driver.h"

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



int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}