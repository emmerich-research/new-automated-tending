#include "device.hpp"

#include "init.hpp"

#include "gpio.hpp"

#include "analog.hpp"
#include "digital.hpp"
#include "pwm.hpp"
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

  auto* analog_device_registry = AnalogDeviceRegistry::get();

  status = analog_device_registry->create<analog::PCF8591Device>(id::analog());
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  return status;
}

static ATM_STATUS initialize_plc_to_pi_comm() {
  auto*      config = Config::get();
  ATM_STATUS status = ATM_OK;

  auto* digital_input_registry = DigitalInputDeviceRegistry::get();

  status = digital_input_registry->create(
      id::comm::plc::tending_height(),
      config->plc_to_pi<PI_PIN>("tending-height", "pin"),
      config->plc_to_pi<bool>("tending-height", "active-state"));
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  status = digital_input_registry->create(
      id::comm::plc::spraying_height(),
      config->plc_to_pi<PI_PIN>("spraying-height", "pin"),
      config->plc_to_pi<bool>("spraying-height", "active-state"));
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  status = digital_input_registry->create(
      id::comm::plc::reset(), config->plc_to_pi<PI_PIN>("reset", "pin"),
      config->plc_to_pi<bool>("reset", "active-state"));
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  status = digital_input_registry->create(
      id::comm::plc::e_stop(), config->plc_to_pi<PI_PIN>("e-stop", "pin"),
      config->plc_to_pi<bool>("e-stop", "active-state"));
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  return status;
}

static ATM_STATUS initialize_limit_switches() {
  // all limit switches are pulled up by default

  auto*      config = Config::get();
  ATM_STATUS status = ATM_OK;

  auto* digital_input_registry = DigitalInputDeviceRegistry::get();

  status = digital_input_registry->create(
      id::limit_switch::x(), config->limit_switch_x<PI_PIN>("pin"),
      config->limit_switch_x<bool>("active-state"), PI_PUD_UP);
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  status = digital_input_registry->create(
      id::limit_switch::y(), config->limit_switch_y<PI_PIN>("pin"),
      config->limit_switch_y<bool>("active-state"), PI_PUD_UP);
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  status = digital_input_registry->create(
      id::limit_switch::z1(), config->limit_switch_z1<PI_PIN>("pin"),
      config->limit_switch_z1<bool>("active-state"), PI_PUD_UP);
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  status = digital_input_registry->create(
      id::limit_switch::z2(), config->limit_switch_z2<PI_PIN>("pin"),
      config->limit_switch_z2<bool>("active-state"), PI_PUD_UP);
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  return status;
}

static ATM_STATUS initialize_input_digital_devices() {
  auto*      config = Config::get();
  ATM_STATUS status = ATM_OK;

  status = DigitalInputDeviceRegistry::create();
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  status = initialize_limit_switches();
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  status = initialize_plc_to_pi_comm();
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  return status;
}

static ATM_STATUS initialize_pi_to_plc_comm() {
  auto       config = Config::get();
  ATM_STATUS status = ATM_OK;

  auto* digital_output_registry = DigitalOutputDeviceRegistry::get();

  status = digital_output_registry->create(
      id::comm::pi::tending_ready(),
      config->pi_to_plc<PI_PIN>("tending-ready", "pin"),
      config->pi_to_plc<bool>("tending-ready", "active-state"));
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  status = digital_output_registry->create(
      id::comm::pi::spraying_ready(),
      config->pi_to_plc<PI_PIN>("spraying-ready", "pin"),
      config->pi_to_plc<bool>("spraying-ready", "active-state"));
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  status = digital_output_registry->create(
      id::comm::pi::tending_running(),
      config->pi_to_plc<PI_PIN>("tending-running", "pin"),
      config->pi_to_plc<bool>("tending-running", "active-state"));
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  status = digital_output_registry->create(
      id::comm::pi::spraying_running(),
      config->pi_to_plc<PI_PIN>("spraying-running", "pin"),
      config->pi_to_plc<bool>("spraying-running", "active-state"));
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  status = digital_output_registry->create(
      id::comm::pi::tending_complete(),
      config->pi_to_plc<PI_PIN>("tending-complete", "pin"),
      config->pi_to_plc<bool>("tending-complete", "active-state"));
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  status = digital_output_registry->create(
      id::comm::pi::spraying_complete(),
      config->pi_to_plc<PI_PIN>("spraying-complete", "pin"),
      config->pi_to_plc<bool>("spraying-complete", "active-state"));
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

  status = initialize_pi_to_plc_comm();
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  return status;
}

static ATM_STATUS initialize_pwm_devices() {
  auto       config = Config::get();
  ATM_STATUS status = ATM_OK;

  status = PWMDeviceRegistry::create();
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  auto* pwm_registry = PWMDeviceRegistry::get();

  status = pwm_registry->create(id::spray(), config->spray<PI_PIN>("pin"),
                                config->spray<bool>("active-state"));
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  auto&& spray = pwm_registry->get(id::spray());

  if (spray->duty_cycle(config->spray<unsigned int>("duty-cycle")) == ATM_ERR) {
    LOG_INFO("Cannot set spray duty cycle...");
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

  auto* stepper_registry = StepperRegistry::get();

  status = stepper_registry->create<LinearSpeedA4988Device>(
      id::stepper::x(), config->stepper_x<PI_PIN>("step-pin"),
      config->stepper_x<PI_PIN>("dir-pin"),
      config->stepper_x<PI_PIN>("enable-pin"));
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  status = stepper_registry->create<LinearSpeedA4988Device>(
      id::stepper::y(), config->stepper_y<PI_PIN>("step-pin"),
      config->stepper_y<PI_PIN>("dir-pin"),
      config->stepper_y<PI_PIN>("enable-pin"));
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  status = stepper_registry->create<LinearSpeedA4988Device>(
      id::stepper::z(), config->stepper_z<PI_PIN>("step-pin"),
      config->stepper_z<PI_PIN>("dir-pin"),
      config->stepper_z<PI_PIN>("enable-pin"));
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  // set microsteps and rpm
  auto&& stepper_x = stepper_registry->get(id::stepper::x());
  stepper_x->microsteps(config->stepper_x<const stepper::step>("microsteps"));
  stepper_x->rpm(config->stepper_x<double>("rpm"));

  auto&& stepper_y = stepper_registry->get(id::stepper::y());
  stepper_y->microsteps(config->stepper_y<const stepper::step>("microsteps"));
  stepper_y->rpm(config->stepper_y<double>("rpm"));

  auto&& stepper_z = stepper_registry->get(id::stepper::z());
  stepper_z->microsteps(config->stepper_z<const stepper::step>("microsteps"));
  stepper_z->rpm(config->stepper_z<double>("rpm"));

  return status;
}

ATM_STATUS initialize_device() {
  if (gpioInitialise() < 0) {
    return ATM_ERR;
  }

  ATM_STATUS status = ATM_OK;

  LOG_INFO("Initializing analog devices...");
  status = initialize_analog_devices();
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  LOG_INFO("Initializing input digital devices...");
  status = initialize_input_digital_devices();
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  LOG_INFO("Initializing output digital devices...");
  status = initialize_output_digital_devices();
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  LOG_INFO("Initializing pwm devices...");
  status = initialize_pwm_devices();
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  LOG_INFO("Initializing stepper devices...");
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
