#include "core.hpp"

#include "init.hpp"

#include "config.hpp"
#include "logger.hpp"
#include "state.hpp"

NAMESPACE_BEGIN

ATM_STATUS initialize_core() {
  ATM_STATUS status = ATM_OK;

  status = Config::create(PROJECT_CONFIG_FILE);
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  status = State::create();
  if (status == ATM_ERR) {
    return ATM_ERR;
  }

  // status = Logger::create(emmerich::Config::get());
  // if (status == ATM_ERR) {
  //   return ATM_ERR;
  // }
  // Logger::get()->info("Booting up...");

  return ATM_OK;
}

void destroy_core() {}

NAMESPACE_END
