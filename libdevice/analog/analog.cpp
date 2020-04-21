#include "device.hpp"

#include "gpio.hpp"

#include "analog/analog.hpp"

NAMESPACE_BEGIN

namespace device {
AnalogDevice::AnalogDevice(unsigned char address,
                           unsigned char bus,
                           unsigned char flags)
    : address_(address), bus_(bus), flags_(flags) {
  handle_ = i2cOpen(bus, address, flags);
}

AnalogDevice::~AnalogDevice() {
  i2cClose(static_cast<unsigned int>(handle_));
}

int AnalogDevice::writeDevice(char* buf, unsigned int count) {
  return i2cWriteDevice(static_cast<unsigned int>(handle_), buf, count);
}

int AnalogDevice::readDevice(char* buf, unsigned int count) {
  return i2cReadDevice(static_cast<unsigned int>(handle_), buf, count);
}

int AnalogDevice::writeByte(unsigned int val) {
  return i2cWriteByte(static_cast<unsigned int>(handle_), val);
}

int AnalogDevice::readByte() {
  return i2cReadByte(static_cast<unsigned int>(handle_));
}
}  // namespace device

NAMESPACE_END
