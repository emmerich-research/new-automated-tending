#ifndef LIB_CORE_ALLOCATION_HPP_
#define LIB_CORE_ALLOCATION_HPP_

/** @file allocation.hpp
 *  @brief Allocation class definitions
 *
 * All object in this project must extend one of this class
 */

#include <cstddef>
#include <memory>
#include <string>
#include <utility>

#include "common.hpp"

NAMESPACE_BEGIN

#ifdef NDEBUG
/**
 * @def ALLOC_SUPER
 *
 * Grand ... grandparent of all class except singleton
 */
#define ALLOC_SUPER : public AllocObj

/**
 * @brief Allocated object super class.
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
  inline const std::string& obj_name() const;

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

#else
/**
 * @def ALLOC_SUPER
 *
 * Grand ... grandparent of all class except singleton which is nothing...
 */
#define ALLOC_SUPER
#endif

/**
 * @brief Parent of all stack-allocated object
 *
 * Stack object, raw pointer creation is prohibited
 * However smart pointer creation is allowed as long as the scope is
 * known (not global like singleton, @see StaticObj)
 *
 * All classes that satisfy that condition MUST extend this class
 *
 * What is the known scope?
 * - Inside the class lifetime
 * - Inside the function
 * - Inside the curly braces
 *
 * @author Ray Andrew
 * @date   April 2020
 */
class StackObj ALLOC_SUPER {
 protected:
  /**
   * Delete `new` operator to disable pointer creation
   */
  void* operator new(std::size_t size) = delete;
  /**
   * Delete `new` operator to disable pointer creation
   */
  void* operator new[](std::size_t size) = delete;
  /**
   * Delete `delete[]` operator to disable pointer deletion
   */
  void operator delete[](void* p) = delete;
};

/**
 * @brief Parent of all singleton class
 *
 * @tparam T class type to instantiate with
 *
 * @author Ray Andrew
 * @date   April 2020
 */
template <typename T>
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
  /**
   * Singleton initialization
   *
   * @param  args    arguments are same with typename T constructor
   * @return T pointer
   */
  template <typename... Args>
  inline static ATM_STATUS create(Args&&... args);
  /**
   * Get T pointer
   *
   * @return T pointer that has been initialized
   */
  inline static T* get();

 private:
  /**
   * T singleton pointer
   */
  static T* instance_;
};

/**
 * @def MAKE_STD_SHARED(T)
 *
 * Create shared_ptr out of any class (even with private constructor and
 * destructor)
 *
 * This macro will generate the static function called `create`
 * that returns shared_ptr<T>
 *
 * Purpose:
 * Hide constructor and destructor so it cannot be invoked
 *
 * Credit:
 * https://stackoverflow.com/a/27832765/6808347
 *
 * Usage:
 * MAKE_STD_SHARED(SomeClass)
 *
 * @param T class to be enabled
 */
#define MAKE_STD_SHARED(T)                                                  \
  template <typename... Args>                                               \
  inline static auto create(Args&&... args) {                               \
    struct EnableMakeShared : public T {                                    \
      EnableMakeShared(Args&&... args) : T(std::forward<Args>(args)...) {}  \
    };                                                                      \
    return std::make_shared<EnableMakeShared>(std::forward<Args>(args)...); \
  }

/**
 * @def MAKE_STD_UNIQUE(T)
 *
 * Create unique_ptr out of any class (even with private constructor and
 * destructor)
 *
 * This macro will generate the static function called `create_unique`
 * that returns unique_ptr<T>
 *
 * Purpose:
 * Hide constructor and destructor so it cannot be invoked
 *
 * Credit:
 * https://stackoverflow.com/a/27832765/6808347
 *
 * Usage:
 * MAKE_STD_UNIQUE(SomeClass)
 *
 * @param T class to be enabled
 */
#define MAKE_STD_UNIQUE(T)                                                  \
  template <typename... Args>                                               \
  inline static auto create_unique(Args&&... args) {                        \
    struct EnableMakeUnique : public T {                                    \
      EnableMakeUnique(Args&&... args) : T(std::forward<Args>(args)...) {}  \
    };                                                                      \
    return std::make_unique<EnableMakeUnique>(std::forward<Args>(args)...); \
  }

NAMESPACE_END

#endif
