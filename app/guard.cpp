#include "precompiled.hpp"

#include "guard.hpp"

#include <libmechanism/mechanism.hpp>
#include <libutil/util.hpp>

NAMESPACE_BEGIN

namespace guard {
bool homing::check() const {
  massert(mechanism::movement_mechanism() != nullptr, "sanity");
  massert(mechanism::movement_mechanism()->active(), "sanity");

  return mechanism::movement_mechanism()->is_home();
}

bool e_stop::check() const {
  auto*  input_registry = device::DigitalInputDeviceRegistry::get();
  auto&& e_stop = input_registry->get(device::id::comm::plc::e_stop());
  return e_stop->read_bool();
}

bool reset::check() const {
  auto*  input_registry = device::DigitalInputDeviceRegistry::get();
  auto&& reset = input_registry->get(device::id::comm::plc::reset());
  return reset->read_bool();
}

namespace spraying {
bool height::check() const {
  auto*  input_registry = device::DigitalInputDeviceRegistry::get();
  auto&& spraying_height =
      input_registry->get(device::id::comm::plc::spraying_height());
  return spraying_height->read_bool();
}
}  // namespace spraying

namespace tending {
bool height::check() const {
  auto*  input_registry = device::DigitalInputDeviceRegistry::get();
  auto&& tending_height =
      input_registry->get(device::id::comm::plc::tending_height());
  return tending_height->read_bool();
}
}  // namespace tending
}  // namespace guard

NAMESPACE_END
