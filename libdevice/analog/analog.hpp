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
 * with library Pigpio
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
   * Write data from analog pin via i2c port
   * Differs between each analog device
   */
  virtual int write(unsigned char pin, unsigned int val) = 0;
  /**
   * Read data from analog pin via i2c port
   * Differs between each analog device
   */
  virtual int read(unsigned char pin) = 0;
  /**
   * Create shared_ptr<AnalogDevice>
   */
  template <typename... Args>
  static auto create(Args&&... args) {
    return std::make_shared<AnalogDevice>(std::forward<Args>(args)...);
  }

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
   */
  virtual int writeDevice(char* buf, unsigned count);
  /**
   * Read count bytes read from the raw device associated with handle via Pigpio
   * lib
   */
  virtual int readDevice(char* buf, unsigned count);
  /**
   * Write single byte to device associated with handle via Pigpio
   * lib
   */
  virtual int writeByte(unsigned val);
  /**
   * Read single byte to device associated with handle via Pigpio
   * lib
   */
  virtual int readByte();

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
