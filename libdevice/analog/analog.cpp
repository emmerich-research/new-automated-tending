#include "device.hpp"

#include "analog/analog.hpp"

#include "gpio.hpp"

NAMESPACE_BEGIN

namespace device {
AnalogDevice::AnalogDevice(unsigned char address,
                           unsigned char bus,
                           unsigned char flags)
    : address_{address}, bus_{bus}, flags_{flags} {
  DEBUG_ONLY(obj_name_ = "AnalogDevice");
  LOG_DEBUG(
      "Initializing AnalogDevice using i2c with address {}, bus {}, and flags "
      "{}",
      address, bus, flags);
  handle_ = i2cOpen(bus, address, flags);
}

AnalogDevice::~AnalogDevice() {
  LOG_DEBUG(
      "Closing AnalogDevice using i2c with address {}, bus {}, and flags {} "
      "with handle {}",
      address_, bus_, flags_, handle_);
  PI_RES res = i2cClose(static_cast<unsigned int>(handle_));

  if (res != PI_OK) {
    LOG_DEBUG("Failed to close i2c device");
  }
}

ATM_STATUS AnalogDevice::write_device(char* buf, unsigned int count) {
  PI_RES res = i2cWriteDevice(static_cast<unsigned int>(handle_), buf, count);

  if (res == PI_OK) {
    return ATM_OK;
  }

  LOG_DEBUG(
      "[FAILED] AnalogDevice::writeDevice with handle {} and buf count {}, "
      "result = {}",
      handle_, count, res);
  return ATM_ERR;
}

int AnalogDevice::read_device(char* buf, unsigned int count) {
  PI_RES res = i2cReadDevice(static_cast<unsigned int>(handle_), buf, count);

  if (res > 0) {
    return res;
  }

  LOG_DEBUG(
      "[FAILED] AnalogDevice::readDevice with handle {} and buf count {}, "
      "result = {}",
      handle_, count, res);
  return ATM_ERR;
}

ATM_STATUS AnalogDevice::write_byte(unsigned int val) {
  PI_RES res = i2cWriteByte(static_cast<unsigned int>(handle_), val);

  if (res == PI_OK) {
    return ATM_OK;
  }

  LOG_DEBUG(
      "[FAILED] AnalogDevice::writeByte with handle {} and val {}, "
      "result = {}",
      handle_, val, res);
  return ATM_ERR;
}

int AnalogDevice::read_byte() {
  PI_RES res = i2cReadByte(static_cast<unsigned int>(handle_));

  if (res == PI_BAD_HANDLE || res == PI_I2C_READ_FAILED) {
    LOG_DEBUG("[FAILED] AnalogDevice::readByte with handle {}, result = {}",
              handle_, res);
    return ATM_ERR;
  }

  return res;
}
}  // namespace device

NAMESPACE_END
