#include "device.hpp"

#include "PCF8591.hpp"

NAMESPACE_BEGIN

namespace device {
namespace analog {
PCF8591Device::PCF8591Device() : AnalogDevice(0x48, 1, 0) {
  DEBUG_ONLY(obj_name_ = "PCF8591Device");
}

ATM_STATUS PCF8591Device::write(unsigned char pin, unsigned int val) {
  unsigned char command[2];
  command[0] = 0x40 | (pin & 0x03);
  command[1] = val;
  int res = writeDevice(reinterpret_cast<char*>(&command), 2);

  if (res == PI_OK) {
    return ATM_OK;
  }

  DEBUG_ONLY(
      LOG_INFO("PCF8591::write to pin {} with val {} failed, result = {}", pin,
               val, res));

  return ATM_ERR;
}

ATM_STATUS PCF8591Device::read(unsigned char pin) {
  unsigned char command[1];
  command[0] = 0x40 | (pin & 0x03);
  int res = writeDevice(reinterpret_cast<char*>(&command), 1);

  if (res == PI_OK) {
    res = readByte();

    if (res == PI_BAD_HANDLE || res == PI_I2C_READ_FAILED) {
      DEBUG_ONLY(
          LOG_INFO("PCF8591::read (i2cReadByte) to pin {} failed, result = {}",
                   pin, res));

      return ATM_ERR;
    }

    return res;
  }

  DEBUG_ONLY(LOG_INFO(
      "PCF8591::read (writeDevice) to pin {} failed, result = {}", pin, res));

  return ATM_ERR;
}
}  // namespace analog
}  // namespace analog

NAMESPACE_END
