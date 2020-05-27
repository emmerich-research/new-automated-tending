#ifndef LIB_DEVICE_ANALOG_ANALOG_HPP_
#define LIB_DEVICE_ANALOG_ANALOG_HPP_

/** @file analog.hpp
 *  @brief Analog device class definition
 *
 * Analog device using i2c
 */

#include <memory>

#include <libalgo/algo.hpp>

#include <libcore/core.hpp>

NAMESPACE_BEGIN

namespace device {
// forward declaration
class AnalogDevice;

/** device::AnalogDevice registry singleton class using algo::InstanceRegistry
 */
using AnalogDeviceRegistry = algo::InstanceRegistry<AnalogDevice>;

/**
 * @brief Analog Device class
 *
 * Abstract class for analog device implementation in RaspberryPI-like GPIO
 * with library Pigpio.
 *
 * Every analog implementation class must extend this class
 *
 * @author Ray Andrew
 * @date   April 2020
 */
class AnalogDevice : public StackObj {
  template <class AnalogDevice>
  template <typename... Args>
  friend ATM_STATUS
  StaticObj<algo::impl::InstanceRegistryImpl<AnalogDevice>>::create(
      Args&&... args);

 public:
  /**
   * Abstract function
   *
   * Write data from analog pin via i2c port
   * Differs between each analog device
   *
   * @param   pin   i2c pin
   * @param   val   value to write
   * @return  ATM_OK or ATM_ERR, but not both
   */
  virtual ATM_STATUS write(unsigned char pin, unsigned int val) = 0;
  /*j*
   * Abstract function
   *
   * Read data from analog pin via i2c port
   * Differs between each analog device
   *
   * @param   pin   i2c pin
   * @return  pin data
   */
  virtual int read(unsigned char pin) = 0;

 protected:
  /**
   * AnalogDevice Constructor
   *
   * Initialize the device by opening i2c port
   *
   * @param  address  i2c address
   * @param  bus      i2c bus
   * @param  flags    i2c flags
   */
  AnalogDevice(unsigned char address, unsigned char bus, unsigned char flags);
  /**
   * AnalogDevice Destructor
   *
   * Close the i2c port that has been initialized
   */
  virtual ~AnalogDevice();
  /**
   * Write the data bytes to the raw device associated with handle via Pigpio
   * lib
   *
   * @param  buf   buffer to write to device
   * @param  count buffer length
   *
   * @return ATM_OK or ATM_ERR, but not both
   */
  virtual ATM_STATUS write_device(char* buf, unsigned count);
  /**
   * Read count bytes read from the raw device associated with handle via Pigpio
   * lib
   *
   * @param  buf   buffer to write to device
   * @param  count buffer length
   *
   * @return > 0 or ATM_ERR
   */
  virtual int read_device(char* buf, unsigned count);
  /**
   * Write single byte to device associated with handle via Pigpio
   * lib
   *
   * @param  val   byte to write
   *
   * @return ATM_OK or ATM_ERR, but not both
   */
  virtual ATM_STATUS write_byte(unsigned val);
  /**
   * Read single byte to device associated with handle via Pigpio
   * lib
   *
   * @return >= 0 or ATM_ERR, but not both
   */
  virtual int read_byte();

 protected:
  /**
   * i2c address
   */
  const unsigned char address_;
  /**
   * i2c bus
   */
  const unsigned char bus_;
  /**
   * i2c flags
   */
  const unsigned char flags_;
  /**
   * i2c handle
   * will be initialized in the constructor
   * and destroyed in the destructor
   */
  int handle_;
};
}  // namespace device

NAMESPACE_END

#endif  // LIB_DEVICE_ANALOG_ANALOG_HPP_
