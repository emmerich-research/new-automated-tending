#include <iostream>

#include <libcore/core.hpp>

int main() {
  emmerich::Config::create(PROJECT_CONFIG_FILE);
  emmerich::State::create();
  emmerich::State::get()->print();
  emmerich::Logger::create(emmerich::Config::get());
  emmerich::Logger::get()->info("hello");
  return 0;
}
