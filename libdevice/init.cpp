#include "device.hpp"

#include "init.hpp"

#include "gpio.hpp"

#include "analog.hpp"
#include "digital.hpp"
#include "pwm.hpp"
#include "stepper.hpp"

#include "A4988.hpp"
#include "PCF8591.hpp"

#include "shift_register.hpp"

NAMESPACE_BEGIN

using namespace device;

// NOTE: Early stopping is a must in here

// forward declarations
static ATM_STATUS initialize_analog_devices();
static ATM_STATUS initialize_plc_to_pi_comm();
static ATM_STATUS initialize_limit_switches();
static ATM_STATUS initialize_input_digital_devices();
static ATM_STATUS initialize_output_digital_devices();
static ATM_STATUS initialize_pi_to_plc_comm();
static ATM_STATUS initialize_shift_register_devices();
static ATM_STATUS initialize_pwm_devices();
static ATM_STATUS initialize_stepper_devices();

static ATM_STATUS initialize_analog_devices() {
  ATM_STATUS status = ATM_OK;

  status = PCF8591Device::create();
  if (status == ATM_ERR) {
    return status;
  }

  auto* analog_device = PCF8591Device::get();

  if (analog_device == nullptr) {
    return ATM_ERR;
  }

  return status;
}

static ATM_STATUS initialize_plc_to_pi_comm() {
  auto*      config = Config::get();
  ATM_STATUS status = ATM_OK;

  auto* digital_input_registry = DigitalInputDeviceRegistry::get();

  status = digital_input_registry->create(
      id::comm::plc::spraying_tending_height(),
      config->plc_to_pi<PI_PIN>("spraying-tending-height", "pin"),
      config->plc_to_pi<bool>("spraying-tending-height", "active-state"),
      PI_PUD_DOWN);
  if (status == ATM_ERR) {
    return status;
  }

  status = digital_input_registry->create(
      id::comm::plc::cleaning_height(),
      config->plc_to_pi<PI_PIN>("cleaning-height", "pin"),
      config->plc_to_pi<bool>("cleaning-height", "active-state"), PI_PUD_DOWN);
  if (status == ATM_ERR) {
    return status;
  }

  status = digital_input_registry->create(
      id::comm::plc::reset(), config->plc_to_pi<PI_PIN>("reset", "pin"),
      config->plc_to_pi<bool>("reset", "active-state"), PI_PUD_DOWN);
  if (status == ATM_ERR) {
    return status;
  }

  status = digital_input_registry->create(
      id::comm::plc::e_stop(), config->plc_to_pi<PI_PIN>("e-stop", "pin"),
      config->plc_to_pi<bool>("e-stop", "active-state"), PI_PUD_DOWN);
  if (status == ATM_ERR) {
    return status;
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
    return status;
  }

  status = digital_input_registry->create(
      id::limit_switch::y(), config->limit_switch_y<PI_PIN>("pin"),
      config->limit_switch_y<bool>("active-state"), PI_PUD_UP);
  if (status == ATM_ERR) {
    return status;
  }

  status = digital_input_registry->create(
      id::limit_switch::z1(), config->limit_switch_z1<PI_PIN>("pin"),
      config->limit_switch_z1<bool>("active-state"), PI_PUD_UP);
  if (status == ATM_ERR) {
    return status;
  }

  status = digital_input_registry->create(
      id::limit_switch::z2(), config->limit_switch_z2<PI_PIN>("pin"),
      config->limit_switch_z2<bool>("active-state"), PI_PUD_UP);
  if (status == ATM_ERR) {
    return status;
  }

  return status;
}

static ATM_STATUS initialize_input_digital_devices() {
  auto*      config = Config::get();
  ATM_STATUS status = ATM_OK;

  status = DigitalInputDeviceRegistry::create();
  if (status == ATM_ERR) {
    return status;
  }

  status = initialize_limit_switches();
  if (status == ATM_ERR) {
    return status;
  }

  status = initialize_plc_to_pi_comm();
  if (status == ATM_ERR) {
    return status;
  }

  /**
   * Anomaly PIN
   */
  auto* digital_input_registry = DigitalInputDeviceRegistry::get();
  status = digital_input_registry->create(
      id::anomaly(), config->anomaly<PI_PIN>("pin"),
      config->anomaly<bool>("active-state"), PI_PUD_DOWN);
  if (status == ATM_ERR) {
    return status;
  }

  return status;
}

static ATM_STATUS initialize_output_digital_devices() {
  auto*      config = Config::get();
  ATM_STATUS status = ATM_OK;

  status = DigitalOutputDeviceRegistry::create();
  if (status == ATM_ERR) {
    return status;
  }

  auto* digital_output_registry = DigitalOutputDeviceRegistry::get();

  status = digital_output_registry->create(
      id::spray(), config->spray<PI_PIN>("pin"),
      config->spray<bool>("active-state"), PI_PUD_UP);
  if (status == ATM_ERR) {
    return status;
  }

  auto&& spray = digital_output_registry->get(id::spray());
  spray->write(device::digital::value::low);

  /**
   * Anomaly with Pin 18 BCM
   * Always start with mode IN with value 1
   */
  // auto* digital_output_registry = DigitalOutputDeviceRegistry::get();
  // status = digital_output_registry->create("UNK", 18, true);
  // if (status == ATM_ERR) {
  //   return ATM_ERR;
  // }

  return status;
}

static ATM_STATUS initialize_pi_to_plc_comm() {
  auto*      config = Config::get();
  ATM_STATUS status = ATM_OK;

  auto* shift_register = ShiftRegister::get();

  if (shift_register == nullptr) {
    return ATM_ERR;
  }

  status = shift_register->assign(
      id::comm::pi::tending_ready(),
      config->pi_to_plc<byte>("tending-ready", "address"),
      config->pi_to_plc<bool>("tending-ready", "active-state"));
  if (status == ATM_ERR) {
    return status;
  }

  status = shift_register->assign(
      id::comm::pi::spraying_ready(),
      config->pi_to_plc<byte>("spraying-ready", "address"),
      config->pi_to_plc<bool>("spraying-ready", "active-state"));
  if (status == ATM_ERR) {
    return status;
  }

  status = shift_register->assign(
      id::comm::pi::tending_running(),
      config->pi_to_plc<byte>("tending-running", "address"),
      config->pi_to_plc<bool>("tending-running", "active-state"));
  if (status == ATM_ERR) {
    return status;
  }

  status = shift_register->assign(
      id::comm::pi::spraying_running(),
      config->pi_to_plc<byte>("spraying-running", "address"),
      config->pi_to_plc<bool>("spraying-running", "active-state"));
  if (status == ATM_ERR) {
    return status;
  }

  status = shift_register->assign(
      id::comm::pi::tending_complete(),
      config->pi_to_plc<byte>("tending-complete", "address"),
      config->pi_to_plc<bool>("tending-complete", "active-state"));
  if (status == ATM_ERR) {
    return status;
  }

  status = shift_register->assign(
      id::comm::pi::spraying_complete(),
      config->pi_to_plc<byte>("spraying-complete", "address"),
      config->pi_to_plc<bool>("spraying-complete", "active-state"));
  if (status == ATM_ERR) {
    return status;
  }

  return status;
}

static ATM_STATUS initialize_shift_register_devices() {
  auto*      config = Config::get();
  ATM_STATUS status = ATM_OK;

  status = ShiftRegister::create(config->shift_register<PI_PIN>("latch-pin"),
                                 config->shift_register<PI_PIN>("clock-pin"),
                                 config->shift_register<PI_PIN>("data-pin"));
  if (status == ATM_ERR) {
    return status;
  }

  status = initialize_pi_to_plc_comm();
  if (status == ATM_ERR) {
    return status;
  }

  return status;
}

static ATM_STATUS initialize_pwm_devices() {
  auto*      config = Config::get();
  ATM_STATUS status = ATM_OK;

  status = PWMDeviceRegistry::create();
  if (status == ATM_ERR) {
    return status;
  }

  auto* pwm_registry = PWMDeviceRegistry::get();

  status = pwm_registry->create(id::finger(), config->finger<PI_PIN>("pin"),
                                config->finger<bool>("active-state"));
  if (status == ATM_ERR) {
    return status;
  }

  auto&& finger = pwm_registry->get(id::finger());

  if (finger->write(device::digital::value::low) == ATM_ERR) {
    LOG_INFO("Cannot set finger duty cycle...");
  }

  return status;
}

static ATM_STATUS initialize_stepper_devices() {
  auto*      config = Config::get();
  ATM_STATUS status = ATM_OK;

  status = StepperRegistry::create();
  if (status == ATM_ERR) {
    return status;
  }

  auto* stepper_registry = StepperRegistry::get();

  status = stepper_registry->create<LinearSpeedA4988Device>(
      id::stepper::x(), config->stepper_x<PI_PIN>("step-pin"),
      config->stepper_x<PI_PIN>("dir-pin"),
      config->stepper_x<PI_PIN>("enable-pin"));
  if (status == ATM_ERR) {
    return status;
  }

  status = stepper_registry->create<LinearSpeedA4988Device>(
      id::stepper::y(), config->stepper_y<PI_PIN>("step-pin"),
      config->stepper_y<PI_PIN>("dir-pin"),
      config->stepper_y<PI_PIN>("enable-pin"));
  if (status == ATM_ERR) {
    return status;
  }

  status = stepper_registry->create<LinearSpeedA4988Device>(
      id::stepper::z(), config->stepper_z<PI_PIN>("step-pin"),
      config->stepper_z<PI_PIN>("dir-pin"),
      config->stepper_z<PI_PIN>("enable-pin"));
  if (status == ATM_ERR) {
    return status;
  }

  // set additional configurations
  auto&& stepper_x = stepper_registry->get(id::stepper::x());
  stepper_x->microsteps(config->stepper_x<const stepper::step>("microsteps"));
  // stepper_x->rpm(config->stepper_x<double>("rpm"));
  // stepper_x->acceleration(config->stepper_x<double>("acceleration"));
  // stepper_x->deceleration(config->stepper_x<double>("deceleration"));
  stepper_x->step_active_state(config->stepper_x<bool>("step-active-state"));
  stepper_x->dir_active_state(config->stepper_x<bool>("dir-active-state"));
  stepper_x->enable_active_state(
      config->stepper_x<bool>("enable-active-state"));

  auto&& stepper_y = stepper_registry->get(id::stepper::y());
  stepper_y->microsteps(config->stepper_y<const stepper::step>("microsteps"));
  // stepper_y->rpm(config->stepper_y<double>("rpm"));
  // stepper_y->acceleration(config->stepper_y<double>("acceleration"));
  // stepper_y->deceleration(config->stepper_y<double>("deceleration"));
  stepper_y->step_active_state(config->stepper_y<bool>("step-active-state"));
  stepper_y->dir_active_state(config->stepper_y<bool>("dir-active-state"));
  stepper_y->enable_active_state(
      config->stepper_y<bool>("enable-active-state"));

  auto&& stepper_z = stepper_registry->get(id::stepper::z());
  stepper_z->microsteps(config->stepper_z<const stepper::step>("microsteps"));
  // stepper_z->rpm(config->stepper_z<double>("rpm"));
  // stepper_z->acceleration(config->stepper_z<double>("acceleration"));
  // stepper_z->deceleration(config->stepper_z<double>("deceleration"));
  stepper_z->step_active_state(config->stepper_z<bool>("step-active-state"));
  stepper_z->dir_active_state(config->stepper_z<bool>("dir-active-state"));
  stepper_z->enable_active_state(
      config->stepper_z<bool>("enable-active-state"));

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
    return status;
  }

  LOG_INFO("Initializing input digital devices...");
  status = initialize_input_digital_devices();
  if (status == ATM_ERR) {
    return status;
  }

  LOG_INFO("Initializing output digital devices...");
  status = initialize_output_digital_devices();
  if (status == ATM_ERR) {
    return status;
  }

  LOG_INFO("Initializing shift register-connected devices...");
  status = initialize_shift_register_devices();
  if (status == ATM_ERR) {
    return status;
  }

  LOG_INFO("Initializing pwm devices...");
  status = initialize_pwm_devices();
  if (status == ATM_ERR) {
    return status;
  }

  LOG_INFO("Initializing stepper devices...");
  status = initialize_stepper_devices();
  if (status == ATM_ERR) {
    return status;
  }

  return status;
}

void destroy_device() {
  gpioTerminate();
}

NAMESPACE_END
