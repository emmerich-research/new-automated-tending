#include "init.hpp"

#include "analog/PCF8591.hpp"
#include "analog/analog.hpp"

NAMESPACE_BEGIN

static ATM_STATUS initialize_analog_devices() {
  return device::AnalogDeviceRegistry::get()
      ->create<device::analog::PCF8591Device>("PCF8591");
}

static ATM_STATUS initialize_digital_devices() {
  return ATM_OK;
}

ATM_STATUS initialize_device() {
  ATM_STATUS status = ATM_OK;

  status = device::AnalogDeviceRegistry::create();

  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  status = initialize_analog_devices();

  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  status = initialize_digital_devices();

  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  return ATM_OK;
}

NAMESPACE_END
