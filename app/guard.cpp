#include "precompiled.hpp"

#include "guard.hpp"

#include <libutil/util.hpp>

NAMESPACE_BEGIN

namespace guard {
bool homing::check() const {
  auto*  input_registry = device::DigitalInputDeviceRegistry::get();
  auto&& limit_switch_x = input_registry->get(device::id::limit_switch::x());
  auto&& limit_switch_y = input_registry->get(device::id::limit_switch::y());
  auto&& limit_switch_z2 = input_registry->get(device::id::limit_switch::z2());
  return util::and_(limit_switch_x->read_bool(), limit_switch_y->read_bool(),
                    limit_switch_z2->read_bool());
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
