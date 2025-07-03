#include "device_driver.h"

DeviceDriver::DeviceDriver(FlashMemoryDevice* hardware) : m_hardware(hardware)
{
}

int DeviceDriver::read(long address)
{
    auto firstReadValue = (int)(m_hardware->read(address));
    const int MAX_REPEAT_CNT = 5 - 1;
    for (int repeatCnt = 0; repeatCnt < 4; repeatCnt++) {
        auto nextReadValue = (int)(m_hardware->read(address));
        if (firstReadValue != nextReadValue) {
            throw ReadFailException();
        }

    }
    return firstReadValue;
}

void DeviceDriver::write(long address, int data)
{
    // TODO: implement this method
    m_hardware->write(address, (unsigned char)data);
}