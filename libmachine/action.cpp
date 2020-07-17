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
  // destroy_gui();
  destroy_core();
  std::cout << "Shutting down is completed!" << std::endl;
}
}  // namespace action

NAMESPACE_END
