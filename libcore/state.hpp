#ifndef LIB_CORE_STATE_HPP
#define LIB_CORE_STATE_HPP

#include <iostream>

#include "common.hpp"
#include "allocation.hpp"

NAMESPACE_BEGIN

namespace impl {
class StateImpl {
public:
  StateImpl() {
    
  }

  void print() {
    std::cout << "hello world" << std::endl;
  }
};
}

class State: public StaticObj<impl::StateImpl, State> {
public:
  State() : StaticObj() {}
  static impl::StateImpl *init() { return new impl::StateImpl(); }
};

NAMESPACE_END

#endif
