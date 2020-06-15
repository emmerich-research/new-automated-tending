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

namespace stepper {
static std::string x_;
static auto        x = []() {
  if (x_.empty()) {
    x_ = Config::get()->stepper_x<std::string>("key");
  }
  return x_;
};

static std::string y_;
static auto        y = []() {
  if (y_.empty()) {
    y_ = Config::get()->stepper_y<std::string>("key");
  }
  return y_;
};

static std::string z_;
static auto        z = []() {
  if (z_.empty()) {
    z_ = Config::get()->stepper_z<std::string>("key");
  }
  return z_;
};
}  // namespace stepper

namespace limit_switch {
static std::string x_;
static auto        x = []() {
  if (x_.empty()) {
    x_ = Config::get()->limit_switch_x<std::string>("key");
  }
  return x_;
};

static std::string y_;
static auto        y = []() {
  if (y_.empty()) {
    y_ = Config::get()->limit_switch_y<std::string>("key");
  }
  return y_;
};

// upper bound
static std::string z1_;
static auto        z1 = []() {
  if (z1_.empty()) {
    z1_ = Config::get()->limit_switch_z1<std::string>("key");
  }
  return z1_;
};

// lower bound
static std::string z2_;
static auto        z2 = []() {
  if (z2_.empty()) {
    z2_ = Config::get()->limit_switch_z2<std::string>("key");
  }
  return z2_;
};
}  // namespace limit_switch

static std::string spray_;
static auto        spray = []() {
  if (spray_.empty()) {
    spray_ = Config::get()->spray<std::string>("key");
  }
  return spray_;
};

namespace comm {
namespace plc {
static std::string tending_height_;
static auto        tending_height = []() {
  if (tending_height_.empty()) {
    tending_height_ =
        Config::get()->plc_to_pi<std::string>("tending-height", "key");
  }
  return tending_height_;
};

static std::string spraying_height_;
static auto        spraying_height = []() {
  if (spraying_height_.empty()) {
    spraying_height_ =
        Config::get()->plc_to_pi<std::string>("spraying-height", "key");
  }
  return spraying_height_;
};

static std::string reset_;
static auto        reset = []() {
  if (reset_.empty()) {
    reset_ = Config::get()->plc_to_pi<std::string>("reset", "key");
  }
  return reset_;
};

static std::string e_stop_;
static auto        e_stop = []() {
  if (e_stop_.empty()) {
    e_stop_ = Config::get()->plc_to_pi<std::string>("e-stop", "key");
  }
  return e_stop_;
};
}  // namespace plc

namespace pi {
static std::string tending_ready_;
static auto        tending_ready = []() {
  if (tending_ready_.empty()) {
    tending_ready_ =
        Config::get()->pi_to_plc<std::string>("tending-ready", "key");
  }
  return tending_ready_;
};

static std::string spraying_ready_;
static auto        spraying_ready = []() {
  if (spraying_ready_.empty()) {
    spraying_ready_ =
        Config::get()->pi_to_plc<std::string>("spraying-ready", "key");
  }
  return spraying_ready_;
};

static std::string tending_running_;
static auto        tending_running = []() {
  if (tending_running_.empty()) {
    tending_running_ =
        Config::get()->pi_to_plc<std::string>("tending-running", "key");
  }
  return tending_running_;
};

static std::string spraying_running_;
static auto        spraying_running = []() {
  if (spraying_running_.empty()) {
    spraying_running_ =
        Config::get()->pi_to_plc<std::string>("spraying-running", "key");
  }
  return spraying_running_;
};

static std::string tending_complete_;
static auto        tending_complete = []() {
  if (tending_complete_.empty()) {
    tending_complete_ =
        Config::get()->pi_to_plc<std::string>("tending-complete", "key");
  }
  return tending_complete_;
};

static std::string spraying_complete_;
static auto        spraying_complete = []() {
  if (spraying_complete_.empty()) {
    spraying_complete_ =
        Config::get()->pi_to_plc<std::string>("spraying-complete", "key");
  }
  return spraying_complete_;
};
}  // namespace pi
}  // namespace comm
}  // namespace id
}  // namespace device

NAMESPACE_END

#endif  // LIB_DEVICE_IDENTIFIER_HPP_
