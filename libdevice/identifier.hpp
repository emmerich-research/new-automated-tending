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
extern std::string   analog_;
static auto          analog = []() {
  if (analog_.empty()) {
    analog_ = Config::get()->analog<std::string>("key");
  }
  return analog_;
};

namespace stepper {
extern std::string x_;
static auto        x = []() {
  if (x_.empty()) {
    x_ = Config::get()->stepper_x<std::string>("key");
  }
  return x_;
};

extern std::string y_;
static auto        y = []() {
  if (y_.empty()) {
    y_ = Config::get()->stepper_y<std::string>("key");
  }
  return y_;
};

extern std::string z_;
static auto        z = []() {
  if (z_.empty()) {
    z_ = Config::get()->stepper_z<std::string>("key");
  }
  return z_;
};
}  // namespace stepper

namespace limit_switch {
extern std::string x_;
static auto        x = []() {
  if (x_.empty()) {
    x_ = Config::get()->limit_switch_x<std::string>("key");
  }
  return x_;
};

extern std::string y_;
static auto        y = []() {
  if (y_.empty()) {
    y_ = Config::get()->limit_switch_y<std::string>("key");
  }
  return y_;
};

// upper bound
extern std::string z1_;
static auto        z1 = []() {
  if (z1_.empty()) {
    z1_ = Config::get()->limit_switch_z1<std::string>("key");
  }
  return z1_;
};

// lower bound
extern std::string z2_;
static auto        z2 = []() {
  if (z2_.empty()) {
    z2_ = Config::get()->limit_switch_z2<std::string>("key");
  }
  return z2_;
};

// finger protection
extern std::string finger_protection_;
static auto        finger_protection = []() {
  if (finger_protection_.empty()) {
    finger_protection_ =
        Config::get()->limit_switch_finger_protection<std::string>("key");
  }
  return finger_protection_;
};
}  // namespace limit_switch

extern std::string spray_;
static auto        spray = []() {
  if (spray_.empty()) {
    spray_ = Config::get()->shift_register<std::string>("spray", "key");
  }
  return spray_;
};

extern std::string finger_;
static auto        finger = []() {
  if (finger_.empty()) {
    finger_ = Config::get()->finger<std::string>("key");
  }
  return finger_;
};

namespace ultrasonic {
extern std::string water_level_;
static auto        water_level = []() {
  if (water_level_.empty()) {
    water_level_ = Config::get()->ultrasonic<std::string>("water-level", "key");
  }
  return water_level_;
};

extern std::string disinfectant_level_;
static auto        disinfectant_level = []() {
  if (disinfectant_level_.empty()) {
    disinfectant_level_ =
        Config::get()->ultrasonic<std::string>("disinfectant-level", "key");
  }
  return disinfectant_level_;
};

}  // namespace ultrasonic

namespace comm {
namespace plc {
extern std::string spraying_tending_height_;
static auto        spraying_tending_height = []() {
  if (spraying_tending_height_.empty()) {
    spraying_tending_height_ =
        Config::get()->plc_to_pi<std::string>("spraying-tending-height", "key");
  }
  return spraying_tending_height_;
};

/**
 * Obsolete
 */
static auto spraying_height = []() {
  if (spraying_tending_height_.empty()) {
    spraying_tending_height_ =
        Config::get()->plc_to_pi<std::string>("spraying-tending-height", "key");
  }
  return spraying_tending_height_;
};

/**
 * Obsolete
 */
static auto tending_height = []() {
  if (spraying_tending_height_.empty()) {
    spraying_tending_height_ =
        Config::get()->plc_to_pi<std::string>("spraying-tending-height", "key");
  }
  return spraying_tending_height_;
};

extern std::string cleaning_height_;
static auto        cleaning_height = []() {
  if (cleaning_height_.empty()) {
    cleaning_height_ =
        Config::get()->plc_to_pi<std::string>("cleaning-height", "key");
  }
  return cleaning_height_;
};

extern std::string reset_;
static auto        reset = []() {
  if (reset_.empty()) {
    reset_ = Config::get()->plc_to_pi<std::string>("reset", "key");
  }
  return reset_;
};

extern std::string e_stop_;
static auto        e_stop = []() {
  if (e_stop_.empty()) {
    e_stop_ = Config::get()->plc_to_pi<std::string>("e-stop", "key");
  }
  return e_stop_;
};
}  // namespace plc

namespace pi {
// PI to PLC
extern std::string tending_ready_;
static auto        tending_ready = []() {
  if (tending_ready_.empty()) {
    tending_ready_ =
        Config::get()->shift_register<std::string>("tending-ready", "key");
  }
  return tending_ready_;
};

extern std::string spraying_ready_;
static auto        spraying_ready = []() {
  if (spraying_ready_.empty()) {
    spraying_ready_ =
        Config::get()->shift_register<std::string>("spraying-ready", "key");
  }
  return spraying_ready_;
};

extern std::string tending_running_;
static auto        tending_running = []() {
  if (tending_running_.empty()) {
    tending_running_ =
        Config::get()->shift_register<std::string>("tending-running", "key");
  }
  return tending_running_;
};

extern std::string spraying_running_;
static auto        spraying_running = []() {
  if (spraying_running_.empty()) {
    spraying_running_ =
        Config::get()->shift_register<std::string>("spraying-running", "key");
  }
  return spraying_running_;
};

extern std::string tending_complete_;
static auto        tending_complete = []() {
  if (tending_complete_.empty()) {
    tending_complete_ =
        Config::get()->shift_register<std::string>("tending-complete", "key");
  }
  return tending_complete_;
};

extern std::string spraying_complete_;
static auto        spraying_complete = []() {
  if (spraying_complete_.empty()) {
    spraying_complete_ =
        Config::get()->shift_register<std::string>("spraying-complete", "key");
  }
  return spraying_complete_;
};

// PI to Cleaning
extern std::string water_in_;
static auto        water_in = []() {
  if (water_in_.empty()) {
    water_in_ = Config::get()->shift_register<std::string>("water-in", "key");
  }
  return water_in_;
};

extern std::string water_out_;
static auto        water_out = []() {
  if (water_out_.empty()) {
    water_out_ = Config::get()->shift_register<std::string>("water-out", "key");
  }
  return water_out_;
};

extern std::string disinfectant_in_;
static auto        disinfectant_in = []() {
  if (disinfectant_in_.empty()) {
    disinfectant_in_ =
        Config::get()->shift_register<std::string>("disinfectant-in", "key");
  }
  return disinfectant_in_;
};

extern std::string disinfectant_out_;
static auto        disinfectant_out = []() {
  if (disinfectant_out_.empty()) {
    disinfectant_out_ =
        Config::get()->shift_register<std::string>("disinfectant-out", "key");
  }
  return disinfectant_out_;
};

extern std::string sonicator_relay_;
static auto        sonicator_relay = []() {
  if (sonicator_relay_.empty()) {
    sonicator_relay_ =
        Config::get()->shift_register<std::string>("sonicator-relay", "key");
  }
  return sonicator_relay_;
};
}  // namespace pi
}  // namespace comm
}  // namespace id
}  // namespace device

NAMESPACE_END

#endif  // LIB_DEVICE_IDENTIFIER_HPP_
