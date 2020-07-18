#include "machine.hpp"

#include "guard.hpp"

#include <libmechanism/mechanism.hpp>
#include <libutil/util.hpp>

NAMESPACE_BEGIN

namespace guard {
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

bool fault::check() const {
  massert(State::get() != nullptr, "sanity");
  return State::get()->fault();
}

namespace spraying {
bool completed::check() const {
  massert(State::get() != nullptr, "sanity");
  return State::get()->spraying_complete();
}
}  // namespace spraying

namespace tending {
bool completed::check() const {
  massert(State::get() != nullptr, "sanity");
  return State::get()->tending_complete();
}
}  // namespace tending

namespace height {
bool spraying_tending::check() const {
  massert(device::DigitalInputDeviceRegistry::get() != nullptr, "sanity");
  auto*  input_registry = device::DigitalInputDeviceRegistry::get();
  auto&& spraying_tending_height =
      input_registry->get(device::id::comm::plc::spraying_tending_height());
  return spraying_tending_height->read_bool();
}

bool cleaning::check() const {
  massert(device::DigitalInputDeviceRegistry::get() != nullptr, "sanity");
  auto*  input_registry = device::DigitalInputDeviceRegistry::get();
  auto&& cleaning_height =
      input_registry->get(device::id::comm::plc::cleaning_height());
  return cleaning_height->read_bool();
}
}  // namespace height
}  // namespace guard

NAMESPACE_END
