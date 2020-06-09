#include "mechanism.hpp"

#include "init.hpp"

#include <memory>

#include "movement.hpp"

NAMESPACE_BEGIN

using namespace mechanism;

static ATM_STATUS initialize_movement_mechanism() {
  auto       config = Config::get();
  ATM_STATUS status = ATM_OK;

  status = MovementBuilder::create();
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  auto movement_builder = MovementBuilder::get();
  status = movement_builder->setup_x(
      device::id::stepper_x(),
      config->stepper_x<const device::stepper::step>("steps-per-mm"),
      device::id::limit_switch_x());
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  status = movement_builder->setup_y(
      device::id::stepper_y(),
      config->stepper_y<const device::stepper::step>("steps-per-mm"),
      device::id::limit_switch_y());
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  status = movement_builder->setup_z(
      device::id::stepper_z(),
      config->stepper_z<const device::stepper::step>("steps-per-mm"),
      device::id::limit_switch_z());
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  [[maybe_unused]] auto movement = movement_builder->build();

  massert(movement_mechanism() != nullptr, "sanity");
  massert(movement_mechanism()->active(), "sanity");

  return status;
}

ATM_STATUS initialize_mechanism() {
  ATM_STATUS status = ATM_OK;

  status = initialize_movement_mechanism();
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  return status;
}

void destroy_mechanism() {}

NAMESPACE_END
