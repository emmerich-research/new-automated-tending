#include "core.hpp"

#include "init.hpp"

#include "config.hpp"
#include "logger.hpp"
#include "state.hpp"

NAMESPACE_BEGIN

ATM_STATUS initialize_core() {
  ATM_STATUS status = ATM_OK;

  status = emmerich::Config::create(emmerich::PROJECT_CONFIG_FILE);
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  status = emmerich::State::create();
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  status = emmerich::Logger::create(emmerich::Config::get());
  if (status == ATM_ERR) {
    return ATM_ERR;
  }
  emmerich::Logger::get()->info("Booting up...");

  return ATM_OK;
}

void destroy_core() {}

NAMESPACE_END
