#ifndef LIB_CORE_ALLOCATION_HPP
#define LIB_CORE_ALLOCATION_HPP

#include "common.hpp"

NAMESPACE_BEGIN

class StackObj {
 private:
  void* operator new(size_t size) = delete;
  void* operator new [](size_t size) = delete;
  void  operator delete(void* p) = delete;
  void  operator delete [](void* p) = delete;
};

// credits to by https://www.theimpossiblecode.com/blog/c11-generic-singleton-pattern/
template<typename T, typename Context>
class StaticObj: StackObj {
 public:
  StaticObj() {
    static bool static_init = []()->bool {
                                instance = Context::init();
                                return true;
                              }();
    (void)static_init;
  }
  // ~StaticObj() = delete;

  T* operator->() { return instance; }
  const T* operator->() const { return instance; }
  T& operator*() { return *instance; }
  const T& operator*() const { return *instance; }

private:
  static T* instance;
};

template<typename T, typename Context>
T* StaticObj<T, Context>::instance;

NAMESPACE_END

#endif
