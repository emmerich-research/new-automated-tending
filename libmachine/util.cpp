#include "machine.hpp"

#include "util.hpp"

#include <libdevice/device.hpp>

NAMESPACE_BEGIN

namespace machine {
namespace util {
void reset_task_state() {
  massert(State::get() != nullptr, "sanity");
  massert(device::ShiftRegister::get() != nullptr, "sanity");
  massert(mechanism::movement_mechanism() != nullptr, "sanity");
  massert(mechanism::movement_mechanism()->active(), "sanity");

  auto*  state = State::get();
  auto*  shift_register = device::ShiftRegister::get();
  auto&& movement = mechanism::movement_mechanism();

  movement->stop_finger();
  movement->disable_motors();

  shift_register->write_all(device::digital::value::low);
  state->reset_ui();
}

void reset_task_ready() {
  massert(State::get() != nullptr, "sanity");
  massert(device::ShiftRegister::get() != nullptr, "sanity");

  auto* state = State::get();
  auto* shift_register = device::ShiftRegister::get();

  shift_register->write(device::id::comm::pi::spraying_ready(),
                        device::digital::value::high);
  shift_register->write(device::id::comm::pi::tending_ready(),
                        device::digital::value::high);

  state->spraying_ready(true);
  state->tending_ready(true);
  state->cleaning_ready(true);
}
}  // namespace util
}  // namespace machine

NAMESPACE_END
