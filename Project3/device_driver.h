#pragma once
#include "flash_memory_device.h"
#include <stdexcept>

class DeviceDriver
{
public:
    DeviceDriver(FlashMemoryDevice* hardware);
    int read(long address);
    void write(long address, int data);
    void postReadConditionCheck(int result, long address);
protected:
    FlashMemoryDevice* m_hardware;
};

class ReadFailException : public std::exception {
public:
    char const* what() const override {
        return "Read value is different";
    }
};
class WriteFailException : public std::exception {
public:
    char const* what() const override {
        return "Already written";
    }
};
