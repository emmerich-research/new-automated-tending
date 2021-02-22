#include "device.hpp"

#include "identifier.hpp"

NAMESPACE_BEGIN

namespace device {
namespace id {
// std::string analog_;

namespace stepper {
std::string x_;
std::string y_;
std::string z_;
}  // namespace stepper

namespace limit_switch {
std::string x_;
std::string y_;
std::string z1_;
std::string z2_;
std::string finger_protection_;
}  // namespace limit_switch

std::string spray_;
std::string finger_;
std::string finger_brake_;
std::string finger_infrared_;
std::string sonicator_relay_;

namespace ultrasonic {
std::string water_level_;
std::string disinfectant_level_;
}  // namespace ultrasonic

namespace float_sensor {
std::string water_level_;
std::string disinfectant_level_;
}  // namespace float_sensor

namespace comm {
namespace plc {
std::string spraying_tending_height_;
std::string cleaning_height_;
std::string reset_;
std::string e_stop_;
}  // namespace plc

namespace pi {
std::string tending_ready_;
std::string spraying_ready_;
std::string tending_running_;
std::string spraying_running_;
std::string tending_complete_;
std::string spraying_complete_;

std::string water_in_;
std::string water_out_;
std::string disinfectant_in_;
std::string disinfectant_out_;
std::string sonicator_relay_;
}  // namespace pi
}  // namespace comm
}  // namespace id
}  // namespace device

NAMESPACE_END
