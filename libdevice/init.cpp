#include "init.hpp"

#include "analog/analog.hpp"

NAMESPACE_BEGIN

ATM_STATUS initialize_device() {
  ATM_STATUS status = ATM_OK;

  status = device::AnalogDeviceRegistry::create();

  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  // device::AnalogDeviceRegistry::get()->create("rot_en", 1, 2, 3);

  return ATM_OK;
}

NAMESPACE_END
