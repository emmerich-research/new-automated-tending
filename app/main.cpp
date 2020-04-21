#include <iostream>

#include <libcore/core.hpp>
#include <libdevice/init.hpp>

int throw_message() {
  std::cerr << "Failed to initialize machine, something wrong" << std::endl;
  return emmerich::ATM_ERR;
}

int main() {
  emmerich::ATM_STATUS status = emmerich::ATM_OK;

  // initialize core such as config, logger, and state
  status = emmerich::initialize_core();
  if (status == emmerich::ATM_ERR) {
    return throw_message();
  }

  // initialize GPIO-based devices such as analog, digital, and PWM
  status = emmerich::initialize_device();
  if (status == emmerich::ATM_ERR) {
    return throw_message();
  }

  return 0;
}
