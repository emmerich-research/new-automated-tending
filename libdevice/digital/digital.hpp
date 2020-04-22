#ifndef LIB_DEVICE_DIGITAL_DIGITAL_HPP_
#define LIB_DEVICE_DIGITAL_DIGITAL_HPP_

#include <libcore/core.hpp>

#include <libalgo/algo.hpp>

NAMESPACE_BEGIN

namespace device {
// forward declaration
class DigitalDevice;

namespace digital {
enum class device_mode {
  INPUT,
  OUTPUT,
  PWM,
};

inline const std::string getDeviceModeString(const device_mode& mode) {
  if (mode == device_mode::INPUT) {
    return "input";
  } else if (mode == device_mode::OUTPUT) {
    return "output";
  } else {
    return "pwm";
  }
}
}  // namespace digital

/** device::DigitalDevice registry singleton class using algo::InstanceRegistry
 */
using DigitalDeviceRegistry = algo::InstanceRegistry<DigitalDevice>;

class DigitalDevice {
 public:
  DigitalDevice(int pin) : _pin(pin) {}
  DigitalDevice(int pin, const device_mode& mode) : _pin(pin), _mode(mode) {}
  virtual ~DigitalDevice() = default;

  // Digital IO
  virtual void          setMode(const device_mode& mode) = 0;
  virtual void          write(const device_output& level) = 0;
  virtual device_output read() const = 0;
  inline int            getPin() const { return _pin; }
  inline device_mode    getMode() const { return _mode; }

 protected:
  const int   pin_;
  device_mode mode_;
};
}  // namespace device

NAMESPACE_END

#endif  // LIB_DEVICE_DIGITAL_DIGITAL_HPP_
