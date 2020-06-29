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
static ATM_STATUS initialize_ultrasonic_devices();

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
  [[maybe_unused]] auto* config = Config::get();
  ATM_STATUS             status = ATM_OK;

  status = DigitalOutputDeviceRegistry::create();
  if (status == ATM_ERR) {
    return status;
  }

  // auto* digital_output_registry = DigitalOutputDeviceRegistry::get();

  // status = digital_output_registry->create(
  //     id::spray(), config->spray<PI_PIN>("pin"),
  //     config->spray<bool>("active-state"), PI_PUD_UP);
  // if (status == ATM_ERR) {
  //   return status;
  // }

  // auto&& spray = digital_output_registry->get(id::spray());
  // spray->write(device::digital::value::low);

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

  // PI to Cleaning Station
  status = shift_register->assign(
      id::comm::pi::water_in(),
      config->shift_register<byte>("water-in", "address"),
      config->shift_register<bool>("water-in", "active-state"));
  if (status == ATM_ERR) {
    return status;
  }

  status = shift_register->assign(
      id::comm::pi::water_out(),
      config->shift_register<byte>("water-out", "address"),
      config->shift_register<bool>("water-out", "active-state"));
  if (status == ATM_ERR) {
    return status;
  }

  status = shift_register->assign(
      id::comm::pi::disinfectant_in(),
      config->shift_register<byte>("disinfectant-in", "address"),
      config->shift_register<bool>("disinfectant-in", "active-state"));
  if (status == ATM_ERR) {
    return status;
  }

  status = shift_register->assign(
      id::comm::pi::disinfectant_out(),
      config->shift_register<byte>("disinfectant-out", "address"),
      config->shift_register<bool>("disinfectant-out", "active-state"));
  if (status == ATM_ERR) {
    return status;
  }

  status = shift_register->assign(
      id::comm::pi::sonicator_relay(),
      config->shift_register<byte>("sonicator-relay", "address"),
      config->shift_register<bool>("sonicator-relay", "active-state"));
  if (status == ATM_ERR) {
    return status;
  }

  // PLC to PI
  status = shift_register->assign(
      id::comm::pi::tending_ready(),
      config->shift_register<byte>("tending-ready", "address"),
      config->shift_register<bool>("tending-ready", "active-state"));
  if (status == ATM_ERR) {
    return status;
  }

  status = shift_register->assign(
      id::comm::pi::spraying_ready(),
      config->shift_register<byte>("spraying-ready", "address"),
      config->shift_register<bool>("spraying-ready", "active-state"));
  if (status == ATM_ERR) {
    return status;
  }

  status = shift_register->assign(
      id::comm::pi::tending_running(),
      config->shift_register<byte>("tending-running", "address"),
      config->shift_register<bool>("tending-running", "active-state"));
  if (status == ATM_ERR) {
    return status;
  }

  status = shift_register->assign(
      id::comm::pi::spraying_running(),
      config->shift_register<byte>("spraying-running", "address"),
      config->shift_register<bool>("spraying-running", "active-state"));
  if (status == ATM_ERR) {
    return status;
  }

  status = shift_register->assign(
      id::comm::pi::tending_complete(),
      config->shift_register<byte>("tending-complete", "address"),
      config->shift_register<bool>("tending-complete", "active-state"));
  if (status == ATM_ERR) {
    return status;
  }

  status = shift_register->assign(
      id::comm::pi::spraying_complete(),
      config->shift_register<byte>("spraying-complete", "address"),
      config->shift_register<bool>("spraying-complete", "active-state"));
  if (status == ATM_ERR) {
    return status;
  }

  status = shift_register->assign(
      id::spray(), config->shift_register<byte>("spray", "address"),
      config->shift_register<bool>("spray", "active-state"));
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

static ATM_STATUS initialize_ultrasonic_devices() {
  auto*      config = Config::get();
  ATM_STATUS status = ATM_OK;

  status = UltrasonicDeviceRegistry::create();
  if (status == ATM_ERR) {
    return status;
  }

  auto* ultrasonic_device_registry = UltrasonicDeviceRegistry::get();

  status = ultrasonic_device_registry->create(
      id::ultrasonic::water_level(),
      config->ultrasonic<PI_PIN>("water-level", "echo-pin"),
      config->ultrasonic<PI_PIN>("water-level", "trigger-pin"),
      config->ultrasonic<bool>("water-level", "echo-active-state"),
      config->ultrasonic<bool>("water-level", "trigger-active-state"));
  if (status == ATM_ERR) {
    return status;
  }

  status = ultrasonic_device_registry->create(
      id::ultrasonic::disinfectant_level(),
      config->ultrasonic<PI_PIN>("disinfectant-level", "echo-pin"),
      config->ultrasonic<PI_PIN>("disinfectant-level", "trigger-pin"),
      config->ultrasonic<bool>("disinfectant-level", "echo-active-state"),
      config->ultrasonic<bool>("disinfectant-level", "trigger-active-state"));
  if (status == ATM_ERR) {
    return status;
  }

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

  LOG_INFO("Initializing ultrasonic devices...");
  status = initialize_ultrasonic_devices();
  if (status == ATM_ERR) {
    return status;
  }

  return status;
}

void destroy_device() {
  gpioTerminate();
}

NAMESPACE_END
