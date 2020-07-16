#ifndef LIB_DEVICE_DIGITAL_PWM_HPP_
#define LIB_DEVICE_DIGITAL_PWM_HPP_

/** @file pwm.hpp
 *  @brief PWM device class definition
 *
 * PWM output device using GPIO
 */

#include <optional>

#include <libalgo/algo.hpp>
#include <libcore/core.hpp>

#include "digital.hpp"

NAMESPACE_BEGIN

namespace device {
// forward declaration
class PWMDevice;

/** device::DigitalOutputDevice registry singleton class using
 * algo::InstanceRegistry
 */
using PWMDeviceRegistry = algo::InstanceRegistry<PWMDevice>;

/**
 * @brief PWM Device implementation.
 *
 * PWM will instantiate GPIO Device using the device::DigitalOutputDevice
 *
 * @author Ray Andrew
 * @date   May 2020
 */
class PWMDevice : public DigitalOutputDevice {
 public:
  /**
   * Create shared_ptr<PWMDevice>
   *
   * Pass every args to PWMDevice()
   *
   * @param args arguments that will be passed to PWMDevice()
   */
  MAKE_STD_SHARED(PWMDevice)
  /**
   * Set PWM duty cycle for specific pin
   *
   * @param  duty_cycle new pwm duty cycle
   *
   * @return ATM_OK or ATM_ERR, but not both
   */
  ATM_STATUS duty_cycle(unsigned int duty_cycle);
  /**
   * Get PWM duty cycle for specific pin
   *
   * @return current duty cycle if succeed
   */
  std::optional<unsigned int> duty_cycle() const;
  /**
   * Set PWM duty cycle range for specific pin
   *
   * @param  range new pwm duty cycle range
   *
   * @return ATM_OK or ATM_ERR, but not both
   */
  ATM_STATUS range(unsigned int range);
  /**
   * Get PWM duty cycle range for specific pin
   *
   * @return current duty cycle range if succeed
   */
  std::optional<unsigned int> range() const;
  /**
   * Get PWM duty cycle real range for specific pin
   *
   * @return current duty cycle real range if succeed
   */
  std::optional<unsigned int> real_range() const;
  /**
   * Set PWM frequency for specific pin
   *
   * @param  frequency new pwm frequency
   *
   * @return ATM_OK or ATM_ERR, but not both
   */
  ATM_STATUS frequency(unsigned int frequency);
  /**
   * Get PWM frequency for specific pin
   *
   * @return current frequency if succeed
   */
  std::optional<unsigned int> frequency() const;
  /**
   * Starts hardware PWM on a GPIO at specified frequeuncy and duty cycle
   *
   * @param  frequency  pwm frequency
   * @param  duty_cycle pwm duty cycle
   *
   * @return ATM_OK or ATM_ERR, but not both
   */
  ATM_STATUS hardware(unsigned int frequency, unsigned int duty_cycle);

 private:
  /**
   * PWMDevice Constructor
   *
   * Initialize the pwm device by opening GPIO
   *
   * @param  pin gpio pin, see Raspberry GPIO pinout for details
   * @param  active_state whether the active state is reversed or not
   */
  PWMDevice(PI_PIN pin, const bool& active_state = true);
  /**
   * PWMDevice Constructor
   *
   * Close the pwm device that has been initialized
   */
  virtual ~PWMDevice() override;
};
}  // namespace device

NAMESPACE_END

#endif  // LIB_DEVICE_DIGITAL_PWM_HPP_
