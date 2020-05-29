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

#include "digital/digital.hpp"

NAMESPACE_BEGIN

namespace device {
/**
 * @brief Digital Device implementation.
 *
 * DigitalDevice will instantiate GPIO Device
 *
 * @tparam Mode digital device mode can be input, output, or pwm
 *
 * @author Ray Andrew
 * @date   May 2020
 */
class PWMDevice : public DigitalOutputDevice {
 public:
  /**
   * PWMDevice Constructor
   *
   * Initialize the pwm device by opening GPIO
   *
   * @param  pin gpio pin, see Raspberry GPIO pinout for details
   */
  PWMDevice(unsigned char pin);

  /**
   * PWMDevice Constructor
   *
   * Close the pwm device that has been initialized
   */
  ~PWMDevice() = default;
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
};
}  // namespace device

NAMESPACE_END

#endif  // LIB_DEVICE_DIGITAL_PWM_HPP_
