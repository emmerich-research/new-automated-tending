#include "core.hpp"

#include "state.hpp"

NAMESPACE_BEGIN

namespace impl {
StateImpl::StateImpl() {
  DEBUG_ONLY(obj_name_ = "StateImpl");

  path_id_ = 0;
}

const unsigned int StateImpl::path_id() const {
  return path_id_;
}

void StateImpl::path_id(unsigned int path_id) {
  path_id_ = path_id;
}
}  // namespace impl

NAMESPACE_END
