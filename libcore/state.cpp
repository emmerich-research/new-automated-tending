#include "core.hpp"

#include "config.hpp"
#include "state.hpp"

NAMESPACE_BEGIN

namespace impl {
StateImpl::StateImpl()
    : speed_profile_{config::speed::normal},
      path_id_{0},
      coordinate_{0.0, 0.0, 0.0},
      tending_{},
      spraying_{},
      cleaning_{},
      manual_mode_{false},
      homing_{false} {
  DEBUG_ONLY(obj_name_ = "StateImpl");
}

StateImpl::StateMutex& StateImpl::mutex() {
  return mutex_;
}

unsigned int StateImpl::path_id() {
  const StateImpl::StateLock lock(mutex());
  return path_id_;
}

void StateImpl::path_id(unsigned int path_id) {
  const StateImpl::StateLock lock(mutex());
  path_id_ = path_id;
}

void StateImpl::coordinate(const Coordinate& coordinate) {
  const StateImpl::StateLock lock(mutex());
  coordinate_.x = coordinate.x;
  coordinate_.y = coordinate.y;
  coordinate_.z = coordinate.z;
}

const Coordinate& StateImpl::coordinate() {
  return coordinate_;
}

void StateImpl::reset_coordinate() {
  coordinate({0.0, 0.0, 0.0});
}

void StateImpl::x(const Point& x) {
  const StateImpl::StateLock lock(mutex());
  coordinate_.x = x;
}

void StateImpl::inc_x() {
  const StateImpl::StateLock lock(mutex());
  coordinate_.x += 1.0;
}

void StateImpl::dec_x() {
  const StateImpl::StateLock lock(mutex());
  coordinate_.x -= 1.0;
}

const Point& StateImpl::x() {
  const StateImpl::StateLock lock(mutex());
  return coordinate_.x;
}

void StateImpl::y(const Point& y) {
  const StateImpl::StateLock lock(mutex());
  coordinate_.y = y;
}

void StateImpl::inc_y() {
  const StateImpl::StateLock lock(mutex());
  coordinate_.y += 1.0;
}

void StateImpl::dec_y() {
  const StateImpl::StateLock lock(mutex());
  coordinate_.y -= 1.0;
}

const Point& StateImpl::y() {
  const StateImpl::StateLock lock(mutex());
  return coordinate_.y;
}

void StateImpl::z(const Point& z) {
  const StateImpl::StateLock lock(mutex());
  coordinate_.z = z;
}

void StateImpl::inc_z() {
  const StateImpl::StateLock lock(mutex());
  coordinate_.z += 1.0;
}

void StateImpl::dec_z() {
  const StateImpl::StateLock lock(mutex());
  coordinate_.z -= 1.0;
}

const Point& StateImpl::z() {
  const StateImpl::StateLock lock(mutex());
  return coordinate_.z;
}

const Task& StateImpl ::spraying() {
  const StateImpl::StateLock lock(mutex());
  return spraying_;
}

void StateImpl::spraying_ready(bool ready) {
  const StateImpl::StateLock lock(mutex());
  spraying_.ready = ready;
}

bool StateImpl::spraying_ready() {
  const StateImpl::StateLock lock(mutex());
  return spraying_.ready;
}

void StateImpl::spraying_running(bool running) {
  const StateImpl::StateLock lock(mutex());
  spraying_.running = running;
}

bool StateImpl::spraying_running() {
  const StateImpl::StateLock lock(mutex());
  return spraying_.running;
}

void StateImpl::spraying_complete(bool complete) {
  const StateImpl::StateLock lock(mutex());
  spraying_.complete = complete;
}

bool StateImpl::spraying_complete() {
  const StateImpl::StateLock lock(mutex());
  return spraying_.complete;
}

// void StateImpl::spraying_fault(bool fault) {
//   const StateImpl::StateLock lock(mutex());
//   spraying_.fault = fault;
// }

// bool StateImpl::spraying_fault() {
//   const StateImpl::StateLock lock(mutex());
//   return spraying_.fault;
// }

const Task& StateImpl ::tending() {
  const StateImpl::StateLock lock(mutex());
  return tending_;
}

void StateImpl::tending_ready(bool ready) {
  const StateImpl::StateLock lock(mutex());
  tending_.ready = ready;
}

bool StateImpl::tending_ready() {
  const StateImpl::StateLock lock(mutex());
  return tending_.ready;
}

void StateImpl::tending_running(bool running) {
  const StateImpl::StateLock lock(mutex());
  tending_.running = running;
}

bool StateImpl::tending_running() {
  const StateImpl::StateLock lock(mutex());
  return tending_.running;
}

void StateImpl::tending_complete(bool complete) {
  const StateImpl::StateLock lock(mutex());
  tending_.complete = complete;
}

bool StateImpl::tending_complete() {
  const StateImpl::StateLock lock(mutex());
  return tending_.complete;
}

// void StateImpl::tending_fault(bool fault) {
//   const StateImpl::StateLock lock(mutex());
//   tending_.fault = fault;
// }

// bool StateImpl::tending_fault() {
//   const StateImpl::StateLock lock(mutex());
//   return tending_.fault;
// }

const Task& StateImpl ::cleaning() {
  const StateImpl::StateLock lock(mutex());
  return cleaning_;
}

void StateImpl::cleaning_ready(bool ready) {
  const StateImpl::StateLock lock(mutex());
  cleaning_.ready = ready;
}

bool StateImpl::cleaning_ready() {
  const StateImpl::StateLock lock(mutex());
  return cleaning_.ready;
}

void StateImpl::cleaning_running(bool running) {
  const StateImpl::StateLock lock(mutex());
  cleaning_.running = running;
}

bool StateImpl::cleaning_running() {
  const StateImpl::StateLock lock(mutex());
  return cleaning_.running;
}

void StateImpl::cleaning_complete(bool complete) {
  const StateImpl::StateLock lock(mutex());
  cleaning_.complete = complete;
}

bool StateImpl::cleaning_complete() {
  const StateImpl::StateLock lock(mutex());
  return cleaning_.complete;
}

void StateImpl::fault(bool fault) {
  const StateImpl::StateLock lock(mutex());
  fault_ = fault;
}

bool StateImpl::fault() {
  const StateImpl::StateLock lock(mutex());
  return fault_;
}

void StateImpl::manual_mode(bool manual) {
  const StateImpl::StateLock lock(mutex());
  manual_mode_ = manual;
}

bool StateImpl::manual_mode() {
  const StateImpl::StateLock lock(mutex());
  return manual_mode_;
}

void StateImpl::homing(bool value) {
  const StateImpl::StateLock lock(mutex());
  homing_ = value;
}

bool StateImpl::homing() {
  const StateImpl::StateLock lock(mutex());
  return homing_;
}

void StateImpl::speed_profile(const config::speed& speed_profile) {
  const StateImpl::StateLock lock(mutex());
  speed_profile_ = speed_profile;
}

const config::speed& StateImpl::speed_profile() {
  const StateImpl::StateLock lock(mutex());
  return speed_profile_;
}
}  // namespace impl

NAMESPACE_END
