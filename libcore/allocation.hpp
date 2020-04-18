#ifndef LIB_CORE_ALLOCATION_HPP_
#define LIB_CORE_ALLOCATION_HPP_

/** \file allocation.hpp
 *  \brief Allocation class definitions
 *
 * All object in this project must extend one of this class
 */

#include <cstddef>
#include <string>

#include "common.hpp"
#include "macros.hpp"

NAMESPACE_BEGIN

#ifdef NDEBUG
/**
 * \def ALLOC_SUPER
 * Grandparent of all class
 */
#define ALLOC_SUPER : public AllocObj

/**
 * \brief Allocated object super class.
 *
 * This class is a parent of all class in the universe of this project.
 * This is good for create a uniform class for debugging.
 *
 * @author Ray Andrew
 * @date   April 2020
 */
class AllocObj {
 public:
  /**
   * Get name of this object
   */
  inline const std::string& get_obj_name() const { return obj_name_; }

  /**
   * Stream allocated object info
   *
   * @param   os   stream class
   * @param   obj  AllocObj object
   * @return  stream containing name
   */
  template <typename OStream>
  friend OStream& operator<<(OStream& os, const AllocObj& obj);

 protected:
  std::string obj_name_;
};

template <typename OStream>
OStream& operator<<(OStream& os, const AllocObj& obj) {
  return os << obj.get_obj_name();
}
#else
/** Grandparent of all class */
#define ALLOC_SUPER
#endif

/**
 * @brief Parent of all stack-allocated object
 *
 * Stack object
 *
 * @author Ray Andrew
 * @date   April 2020
 */
class StackObj ALLOC_SUPER {
 private:
  /**
   * Delete `new` operator to disable pointer creation
   */
  void* operator new(std::size_t size) = delete;
  /**
   * Delete `new` operator to disable pointer creation
   */
  void* operator new[](std::size_t size) = delete;
  /**
   * Delete `delete` operator to disable pointer deletion
   */
  void operator delete(void* p) = delete;
  /**
   * Delete `delete` operator to disable pointer deletion
   */
  void operator delete[](void* p) = delete;
};

/**
 * @brief Parent of all singleton class
 *
 * @author Ray Andrew
 * @date   April 2020
 */
class StaticObj {
 public:
  /**
   * Delete constructor because singleton does not need it
   */
  StaticObj() = delete;
  /**
   * Delete destructor because singleton does not need it
   */
  ~StaticObj() = delete;
};

// credits to
// https://www.theimpossiblecode.com/blog/c11-generic-singleton-pattern/
// with some changes by Ray Andrew <raydreww@gmail.com>
// template <typename T, typename CONTEXT>
// class Singleton : public StackObj {
//  public:
//   T* operator->() {
//     massert(instance_ != nullptr, "sanity check");
//     return instance_;
//   }
//   const T* operator->() const {
//     massert(instance_ != nullptr, "sanity check");
//     return instance_;
//   }
//   T& operator*() {
//     massert(instance_ != nullptr, "sanity check");
//     return *instance_;
//   }
//   const T& operator*() const {
//     massert(instance_ != nullptr, "sanity check");
//     return *instance_;
//   }

//   template <typename... Args>
//   Singleton(Args... args) {
//     static bool static_init = [=]() -> bool {
//       instance_ = new T(args...);
//       return true;
//     }();
//     (void)static_init;
//     massert(static_init && instance_ != nullptr, "sanity check");
//   }

//  private:
//   static T* instance_;
// };

// template <typename T, typename Context>
// T* Singleton<T, Context>::instance_;

NAMESPACE_END

#endif
