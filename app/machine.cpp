#include "precompiled.hpp"

#include "machine.hpp"

NAMESPACE_BEGIN

void shutdown_hook() {
  std::cout << "Shutting down..." << std::endl;
  destroy_device();
  destroy_core();
  std::cout << "Shutting down is completed!" << std::endl;
}

namespace machine {
const int TendingDef::VERSION = 1;
}

NAMESPACE_END
