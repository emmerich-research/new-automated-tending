#ifndef LIB_DEVICE_ANALOG_PCF8591_HPP_
#define LIB_DEVICE_ANALOG_PCF8591_HPP_

#include <libcore/core.hpp>

#include "analog.hpp"

NAMESPACE_BEGIN

namespace device {
namespace analog {
class PCF8591Device : public AnalogDevice {
 public:
  /**
   * Write data from analog pin via i2c port
   *
   * @param   pin   i2c pin
   * @param   val   value to write
   * @return  ATM_OK or ATM_ERR, but not both
   */
  virtual ATM_STATUS write(unsigned char pin, unsigned int val);
  /**
   * Read data from analog pin via i2c port
   *
   * @param   pin   i2c pin
   * @return  ATM_OK or ATM_ERR, but not both
   */
  virtual int read(unsigned char pin);

 protected:
  /**
   * PCF8591Device Constructor
   *
   * Initialize the device by opening i2c port
   */
  PCF8591Device();
  /**
   * PCF8591Device Destructor
   *
   * Close the i2c port that has been initialized
   */
  virtual ~PCF8591Device() = default;
};
}  // namespace analog
}  // namespace device

NAMESPACE_END

#endif  // LIB_DEVICE_ANALOG_PCF8591_HPP_
