#ifndef LIB_DEVICE_IDENTIFIER_HPP_
#define LIB_DEVICE_IDENTIFIER_HPP_

/** @file identifier.hpp
 *  @brief Devices Registry Instance IDs
 *
 *  Devices Registry Instance IDs
 */
#include <libcore/core.hpp>

NAMESPACE_BEGIN

namespace device {
namespace id {
static std::string   analog_;
static auto          analog = []() {
  if (analog_.empty()) {
    analog_ = Config::get()->analog<std::string>("key");
  }
  return analog_;
};

static std::string stepper_x_;
static auto        stepper_x = []() {
  if (stepper_x_.empty()) {
    stepper_x_ = Config::get()->stepper_x<std::string>("key");
  }
  return stepper_x_;
};

static std::string stepper_y_;
static auto        stepper_y = []() {
  if (stepper_y_.empty()) {
    stepper_y_ = Config::get()->stepper_y<std::string>("key");
  }
  return stepper_y_;
};

static std::string stepper_z_;
static auto        stepper_z = []() {
  if (stepper_z_.empty()) {
    stepper_z_ = Config::get()->stepper_z<std::string>("key");
  }
  return stepper_z_;
};

static std::string limit_switch_x_;
static auto        limit_switch_x = []() {
  if (limit_switch_x_.empty()) {
    limit_switch_x_ = Config::get()->limit_switch_x<std::string>("key");
  }
  return limit_switch_x_;
};

static std::string limit_switch_y_;
static auto        limit_switch_y = []() {
  if (limit_switch_y_.empty()) {
    limit_switch_y_ = Config::get()->limit_switch_y<std::string>("key");
  }
  return limit_switch_y_;
};

static std::string limit_switch_z_;
static auto        limit_switch_z = []() {
  if (limit_switch_z_.empty()) {
    limit_switch_z_ = Config::get()->limit_switch_z<std::string>("key");
  }
  return limit_switch_z_;
};
}  // namespace id
}  // namespace device

NAMESPACE_END

#endif  // LIB_DEVICE_IDENTIFIER_HPP_
