#include "machine.hpp"

#include "util.hpp"

#include <libdevice/device.hpp>

NAMESPACE_BEGIN

namespace machine {
namespace util {

void reset_task_state() {
  massert(State::get() != nullptr, "sanity");
  massert(device::ShiftRegister::get() != nullptr, "sanity");

  auto* state = State::get();
  auto* shift_register = device::ShiftRegister::get();
  massert(shift_register != nullptr, "sanity");

  shift_register->write(device::id::comm::pi::spraying_ready(),
                        device::digital::value::low);
  state->spraying_ready(false);

  shift_register->write(device::id::comm::pi::spraying_running(),
                        device::digital::value::low);
  state->spraying_running(false);

  shift_register->write(device::id::comm::pi::spraying_complete(),
                        device::digital::value::low);
  state->spraying_complete(false);

  shift_register->write(device::id::comm::pi::tending_ready(),
                        device::digital::value::low);
  state->tending_ready(false);
  shift_register->write(device::id::comm::pi::tending_running(),
                        device::digital::value::low);
  state->tending_running(false);
  shift_register->write(device::id::comm::pi::tending_complete(),
                        device::digital::value::low);
  state->tending_complete(false);

  shift_register->write(device::id::spray(), device::digital::value::low);

  // state->spraying_fault(false);
  // state->tending_fault(false);
}

void reset_task_ready() {
  massert(State::get() != nullptr, "sanity");
  massert(device::ShiftRegister::get() != nullptr, "sanity");

  auto* state = State::get();
  auto* shift_register = device::ShiftRegister::get();

  shift_register->write(device::id::comm::pi::spraying_ready(),
                        device::digital::value::high);
  state->spraying_ready(true);

  shift_register->write(device::id::comm::pi::tending_ready(),
                        device::digital::value::high);
  state->tending_ready(true);

  state->cleaning_ready(true);
}
}  // namespace util
}  // namespace machine

NAMESPACE_END
