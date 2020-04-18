#include "core.hpp"

#include "state.hpp"

NAMESPACE_BEGIN

impl::StateImpl* State::instance_ = nullptr;

namespace impl {
StateImpl::StateImpl() {
  DEBUG_ONLY(obj_name_ = "StateImpl");
}

void StateImpl::print() {
  std::cout << "hello world" << std::endl;
}

}  // namespace impl

NAMESPACE_END
