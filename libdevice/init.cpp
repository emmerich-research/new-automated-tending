#include "init.hpp"

#include "gpio.hpp"

#include "analog.hpp"
#include "digital.hpp"
#include "stepper.hpp"

#include "A4988.hpp"
#include "PCF8591.hpp"

NAMESPACE_BEGIN

using namespace device;

static ATM_STATUS initialize_analog_devices() {
  auto analog_device_registry = AnalogDeviceRegistry::get();
  return analog_device_registry->create<analog::PCF8591Device>("PCF8591");
}

static ATM_STATUS initialize_input_digital_devices() {
  ATM_STATUS status = ATM_OK;
  auto       digital_input_registry = DigitalInputDeviceRegistry::get();

  status = digital_input_registry->create("LIMIT_X", 1);
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  status = digital_input_registry->create("LIMIT_Y", 2);
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  status = digital_input_registry->create("LIMIT_Z", 3);
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  return status;
}

static ATM_STATUS initialize_output_digital_devices() {
  ATM_STATUS status = ATM_OK;
  auto       digital_output_registry = DigitalOutputDeviceRegistry::get();

  status = digital_output_registry->create("OUTPUT1", 4);
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  status = digital_output_registry->create("OUTPUT2", 5);
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  return status;
}

static ATM_STATUS initialize_stepper_devices() {
  ATM_STATUS status = ATM_OK;
  auto       stepper_registry = StepperRegistry::get();

  status =
      stepper_registry->create<LinearSpeedA4988Device>("STEPPER_X", 12, 13, 14);
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  status =
      stepper_registry->create<LinearSpeedA4988Device>("STEPPER_Y", 12, 13, 14);
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  status =
      stepper_registry->create<LinearSpeedA4988Device>("STEPPER_Z", 12, 13, 14);
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  // set microsteps
  auto stepper_x = stepper_registry->get("STEPPER_X");
  DEBUG_ONLY(LOG_DEBUG("STEPPER_X = {}", stepper_x->obj_name()));
  stepper_x->microsteps(1);

  auto stepper_y = stepper_registry->get("STEPPER_Y");
  stepper_y->microsteps(1);

  auto stepper_z = stepper_registry->get("STEPPER_Z");
  stepper_z->microsteps(1);

  return status;
}

ATM_STATUS initialize_device() {
  if (gpioInitialise() < 0)
    return ATM_ERR;

  ATM_STATUS status = ATM_OK;

  status = AnalogDeviceRegistry::create();
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  status = DigitalInputDeviceRegistry::create();
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  status = DigitalOutputDeviceRegistry::create();
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  status = StepperRegistry::create();
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  status = initialize_analog_devices();
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  status = initialize_input_digital_devices();
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  status = initialize_output_digital_devices();
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  status = initialize_stepper_devices();
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  return ATM_OK;
}

void destroy_device() {
  gpioTerminate();
}

NAMESPACE_END
