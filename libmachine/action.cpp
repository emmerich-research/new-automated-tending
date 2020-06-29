#include "machine.hpp"

#include "action.hpp"

#include <exception>

#include <libmechanism/mechanism.hpp>

NAMESPACE_BEGIN

namespace action {
void shutdown_hook() {
  LOG_INFO("Shutting down...");
  auto&& movement = mechanism::movement_mechanism();
  if (movement != nullptr) {
    movement->disable_motors();
  }
  destroy_device();
  destroy_gui();
  destroy_core();
  std::cout << "Shutting down is completed!" << std::endl;
}

void homing::act() const {
  massert(mechanism::movement_mechanism() != nullptr, "sanity");
  massert(mechanism::movement_mechanism()->active(), "sanity");

  LOG_INFO("Homing to make sure task ready...");

  auto&&        movement = mechanism::movement_mechanism();
  guard::homing g_homing;

  // bool is_home = g_homing.check();

  // if (!is_home) {
  movement->homing();
  // }
  // while (!is_home) {
  //   is_home = g_homing.check();
  //   movement->homing();
  //   is_home = g_homing.check();
  //   LOG_DEBUG("Still doing homing, retrying homing for 400ms...");
  //   sleep_for<time_units::millis>(400);
  // }
  // massert(g_homing.check(), "sanity");
}
}  // namespace action

NAMESPACE_END
