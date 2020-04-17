#include "precompiled.hpp"

#include "state.hpp"

#include "common.hpp"

NAMESPACE_BEGIN

impl::StateImpl* State::instance_ = nullptr;

namespace impl {
StateImpl::StateImpl() {}

void StateImpl::print() {
  std::cout << "hello world" << std::endl;
}

}  // namespace impl

NAMESPACE_END
