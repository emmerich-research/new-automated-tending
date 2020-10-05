#include "mechanism.hpp"

#include "init.hpp"

#include <memory>

#include "liquid-refilling.hpp"
#include "movement.hpp"

NAMESPACE_BEGIN

using namespace mechanism;

static ATM_STATUS initialize_movement_mechanism() {
  auto*      config = Config::get();
  ATM_STATUS status = ATM_OK;

  status = MovementBuilder::create();
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  auto movement_builder = MovementBuilder::get();
  status = movement_builder->setup_x(
      device::id::stepper::x(),
      config->stepper_x<const device::stepper::step>("steps-per-mm"),
      device::id::limit_switch::x());
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  status = movement_builder->setup_y(
      device::id::stepper::y(),
      config->stepper_y<const device::stepper::step>("steps-per-mm"),
      device::id::limit_switch::y());
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  status = movement_builder->setup_z(
      device::id::stepper::z(),
      config->stepper_z<const device::stepper::step>("steps-per-mm"),
      device::id::limit_switch::z1(), device::id::limit_switch::z2());
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  status = movement_builder->setup_finger(device::id::finger(),
                                          device::id::finger_brake(),
                                          device::id::finger_infrared());
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  [[maybe_unused]] auto movement = movement_builder->build();

  massert(movement_mechanism() != nullptr, "sanity");
  massert(movement_mechanism()->active(), "sanity");

  status = LiquidRefilling::create();

  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  massert(LiquidRefilling::get() != nullptr, "sanity");

  auto* liquid_refill_mechanism = LiquidRefilling::get();

  liquid_refill_mechanism->setup_water_device(
      device::id::float_sensor::water_level(), device::id::comm::pi::water_in(),
      device::id::comm::pi::water_out());
  liquid_refill_mechanism->setup_disinfectant_device(
      device::id::float_sensor::disinfectant_level(),
      device::id::comm::pi::disinfectant_in(),
      device::id::comm::pi::disinfectant_out());

  liquid_refill_mechanism->setup_draining_time(
      config->liquid_refilling<unsigned int>("water", "draining-time"),
      config->liquid_refilling<unsigned int>("disinfectant", "draining-time"));

  massert(LiquidRefilling::get()->active(), "sanity");

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
