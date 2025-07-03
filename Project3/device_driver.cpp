#include "device_driver.h"

DeviceDriver::DeviceDriver(FlashMemoryDevice* hardware) : m_hardware(hardware)
{
}

int DeviceDriver::read(long address)
{
    auto result = (int)(m_hardware->read(address));
    bool isDifferent = false;
    postReadConditionCheck(result, address);
    return result;
}

void DeviceDriver::postReadConditionCheck(int result, long address)
{
    const int REMAIN_REPEAT_CNT = 4;
    bool isDiffernet = false;
    for (int repeatCnt = 0; repeatCnt < REMAIN_REPEAT_CNT; repeatCnt++) {
        auto currentResult = (int)(m_hardware->read(address));
        if (result != currentResult) throw ReadFailException();
    }
}

void DeviceDriver::write(long address, int data)
{
    checkWritePrecondition(address);
    m_hardware->write(address, (unsigned char)data);
}

void DeviceDriver::checkWritePrecondition(long address)
{
    const int IS_EMPTY = 0xFF;
    auto readValue = (int)(m_hardware->read(address));
    if (readValue != IS_EMPTY) throw WriteFailException();
}
