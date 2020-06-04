#include "mechanism.hpp"

#include "movement.hpp"

NAMESPACE_BEGIN

namespace mechanism {
namespace impl {
MovementBuilderImpl::MovementBuilderImpl(
    const device::stepper::step& steps_per_mm)
    : steps_per_mm_{steps_per_mm} {}

ATM_STATUS MovementBuilderImpl::setup_x(const char* stepper_x_id,
                                        const char* limit_switch_x_id) {
  if (!device::StepperRegistry::get()->exist(stepper_x_id)) {
    return ATM_ERR;
  }

  if (!device::DigitalInputDeviceRegistry::get()->exist(limit_switch_x_id)) {
    return ATM_ERR;
  }

  stepper_x_id_ = stepper_x_id;
  limit_switch_x_id_ = limit_switch_x_id;
  return ATM_OK;
}

ATM_STATUS MovementBuilderImpl::setup_y(const char* stepper_y_id,
                                        const char* limit_switch_y_id) {
  if (!device::StepperRegistry::get()->exist(stepper_y_id)) {
    return ATM_ERR;
  }

  if (!device::DigitalInputDeviceRegistry::get()->exist(limit_switch_y_id)) {
    return ATM_ERR;
  }

  stepper_y_id_ = stepper_y_id;
  limit_switch_y_id_ = limit_switch_y_id;
  return ATM_OK;
}

ATM_STATUS MovementBuilderImpl::setup_z(const char* stepper_z_id,
                                        const char* limit_switch_z_id) {
  if (!device::StepperRegistry::get()->exist(stepper_z_id)) {
    return ATM_ERR;
  }

  if (!device::DigitalInputDeviceRegistry::get()->exist(limit_switch_z_id)) {
    return ATM_ERR;
  }

  stepper_z_id_ = stepper_z_id;
  limit_switch_z_id_ = limit_switch_z_id;
  return ATM_OK;
}

std::shared_ptr<Movement> MovementBuilderImpl::build() {
  return Movement::create(this);
}
}  // namespace impl

Movement::Movement(const impl::MovementBuilderImpl* builder)
    : steps_per_mm_{builder->steps_per_mm()}, active_{true}, thread_pool_{3} {
  setup_stepper(builder);
  if (active()) {
    setup_limit_switch(builder);
  }
}

template <>
long Movement::convert_length_to_steps<movement::unit::cm>(
    double                       length,
    const device::stepper::step& steps_per_mm) {
  return length * steps_per_mm / 100.0;
}

template <>
long Movement::convert_length_to_steps<movement::unit::mm>(
    double                       length,
    const device::stepper::step& steps_per_mm) {
  return length * steps_per_mm;
}

void Movement::setup_stepper(const impl::MovementBuilderImpl* builder) {
  auto stepper_registry = device::StepperRegistry::get();
  auto stepper_x = stepper_registry->get(builder->stepper_x_id());

  if (!stepper_x) {
    active_ = false;
    return;
  }

  stepper_x_ = stepper_x;

  auto stepper_y = stepper_registry->get(builder->stepper_x_id());

  if (!stepper_y) {
    active_ = false;
    return;
  }

  stepper_y_ = stepper_y;

  auto stepper_z = stepper_registry->get(builder->stepper_x_id());

  if (!stepper_z) {
    active_ = false;
    return;
  }

  stepper_z_ = stepper_z;
}

void Movement::setup_limit_switch(const impl::MovementBuilderImpl* builder) {
  auto digital_input_registry = device::DigitalInputDeviceRegistry::get();

  auto limit_switch_x =
      digital_input_registry->get(builder->limit_switch_x_id());

  if (!limit_switch_x) {
    active_ = false;
    return;
  }

  limit_switch_x_ = limit_switch_x;

  auto limit_switch_y =
      digital_input_registry->get(builder->limit_switch_y_id());

  if (!limit_switch_y) {
    active_ = false;
    return;
  }

  limit_switch_y_ = limit_switch_y;

  auto limit_switch_z =
      digital_input_registry->get(builder->limit_switch_z_id());

  if (!limit_switch_z) {
    active_ = false;
    return;
  }

  limit_switch_z_ = limit_switch_z;
}

const device::stepper::step Movement::stop_x(void) {
  return stepper_x()->stop();
}

const device::stepper::step Movement::stop_y(void) {
  return stepper_y()->stop();
}

const device::stepper::step Movement::stop_z(void) {
  return stepper_z()->stop();
}

void Movement::stop(void) {
  [[maybe_unused]] auto step_x = stop_x();
  [[maybe_unused]] auto step_y = stop_y();
  [[maybe_unused]] auto step_z = stop_z();
}
}  // namespace mechanism

NAMESPACE_END
