#ifndef LIB_DEVICE_ANALOG_PCF8591_HPP_
#define LIB_DEVICE_ANALOG_PCF8591_HPP_

/** @file PCF8591.hpp
 *  @brief PCF8591 Analog Digital Converter
 *
 * PCF8591 Analog Digital Converter
 */

#include <libcore/core.hpp>

#include "analog.hpp"

NAMESPACE_BEGIN

namespace device {
namespace analog {
class PCF8591Device;
}

/** device::AnalogDevice registry singleton class using StaticObj
 */
using PCF8591Device = StaticObj<analog::PCF8591Device>;

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
  virtual ATM_STATUS write(const PI_PIN& pin, const analog::value& val) override;
  /**
   * Read data from analog pin via i2c port
   *
   * @param   pin   i2c pin
   * @return  ATM_OK or ATM_ERR, but not both
   */
  virtual std::optional<analog::value> read(const PI_PIN& pin) override;
  /**
   * Create shared_ptr<PCF8591Device>
   *
   * Pass every args to PCF8591Device()
   *
   * @param args arguments that will be passed to PCF8591()
   */
  template <typename... Args>
  inline static auto create(Args&&... args) {
    return std::make_shared<PCF8591Device>(std::forward<Args>(args)...);
  }
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
  virtual ~PCF8591Device() override = default;
};
}  // namespace analog
}  // namespace device

NAMESPACE_END

#endif  // LIB_DEVICE_ANALOG_PCF8591_HPP_
