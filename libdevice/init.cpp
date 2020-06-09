#include "device.hpp"

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
  ATM_STATUS status = ATM_OK;

  status = AnalogDeviceRegistry::create();
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  auto analog_device_registry = AnalogDeviceRegistry::get();

  status = analog_device_registry->create<analog::PCF8591Device>(id::analog());
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  return status;
}

static ATM_STATUS initialize_input_digital_devices() {
  auto       config = Config::get();
  ATM_STATUS status = ATM_OK;

  status = DigitalInputDeviceRegistry::create();
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  auto digital_input_registry = DigitalInputDeviceRegistry::get();

  status = digital_input_registry->create(
      id::limit_switch_x(), config->limit_switch_x<PI_PIN>("pin"),
      config->limit_switch_x<bool>("active-state"));
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  status = digital_input_registry->create(
      id::limit_switch_y(), config->limit_switch_y<PI_PIN>("pin"),
      config->limit_switch_y<bool>("active-state"));
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  status = digital_input_registry->create(
      id::limit_switch_z(), config->limit_switch_z<PI_PIN>("pin"),
      config->limit_switch_z<bool>("active-state"));
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  return status;
}

static ATM_STATUS initialize_output_digital_devices() {
  ATM_STATUS status = ATM_OK;

  status = DigitalOutputDeviceRegistry::create();
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  auto digital_output_registry = DigitalOutputDeviceRegistry::get();

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
  auto       config = Config::get();
  ATM_STATUS status = ATM_OK;

  status = StepperRegistry::create();
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  auto stepper_registry = StepperRegistry::get();

  status = stepper_registry->create<LinearSpeedA4988Device>(
      id::stepper_x(), config->stepper_x<PI_PIN>("step-pin"),
      config->stepper_x<PI_PIN>("dir-pin"),
      config->stepper_x<PI_PIN>("enable-pin"));
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  status = stepper_registry->create<LinearSpeedA4988Device>(
      id::stepper_y(), config->stepper_y<PI_PIN>("step-pin"),
      config->stepper_y<PI_PIN>("dir-pin"),
      config->stepper_y<PI_PIN>("enable-pin"));
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  status = stepper_registry->create<LinearSpeedA4988Device>(
      id::stepper_z(), config->stepper_z<PI_PIN>("step-pin"),
      config->stepper_z<PI_PIN>("dir-pin"),
      config->stepper_z<PI_PIN>("enable-pin"));
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  // set microsteps and rpm
  auto stepper_x = stepper_registry->get(id::stepper_x());
  stepper_x->microsteps(config->stepper_x<const stepper::step>("microsteps"));
  stepper_x->rpm(config->stepper_x<double>("rpm"));

  auto stepper_y = stepper_registry->get(id::stepper_y());
  stepper_y->microsteps(config->stepper_y<const stepper::step>("microsteps"));
  stepper_y->rpm(config->stepper_y<double>("rpm"));

  auto stepper_z = stepper_registry->get(id::stepper_z());
  stepper_z->microsteps(config->stepper_z<const stepper::step>("microsteps"));
  stepper_z->rpm(config->stepper_z<double>("rpm"));

  return status;
}

ATM_STATUS initialize_device() {
  if (gpioInitialise() < 0) {
    return ATM_ERR;
  }

  ATM_STATUS status = ATM_OK;

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
