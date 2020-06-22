#include "core.hpp"

#include "state.hpp"

NAMESPACE_BEGIN

namespace impl {
StateImpl::StateImpl()
    : path_id_{0}, coordinate_{0.0, 0.0, 0.0}, spraying_{}, tending_{} {
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

const Coordinate& StateImpl::coordinate()  {
  return coordinate_;
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

Task& StateImpl ::spraying() {
  const StateImpl::StateLock lock(mutex());
  return spraying_;
}

Task& StateImpl ::tending() {
  const StateImpl::StateLock lock(mutex());
  return tending_;
}
}  // namespace impl

NAMESPACE_END
