#ifndef LIB_ALGO_THREAD_POOL_HPP_
#define LIB_ALGO_THREAD_POOL_HPP_

/** @file thread_pool.hpp
 *  @brief Thread pool class definition
 *
 * ThreadPool Implementation in C++17 for this project
 *
 * Credits to Jakob Progsch (@progschj)
 * https://github.com/progschj/ThreadPool
 *
 * Copyright (c) 2012 Jakob Progsch, Václav Zeman
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 * claim that you wrote the original software. If you use this software
 * in a product, an acknowledgment in the product documentation would be
 * appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 * distribution.
 *
 * Modified by @zserik to support C++17
 * https://github.com/zserik/ThreadPool
 *
 * Modified by @rayandrews to use in this project
 * <raydreww@gmail.com>
 */

#include <cstddef>
#include <functional>
#include <future>
#include <queue>
#include <thread>
#include <vector>

#include <libcore/core.hpp>

NAMESPACE_BEGIN

namespace algo {
/**
 * @brief Thread Pool implementation.
 *
 * ThreadPool will spawn thread based on the number of workers that specified at
 * constructor
 *
 * @author Jakob Progsch, Václav Zeman, zserik, Ray Andrew
 * @date   May 2020
 */
class ThreadPool : public StackObj {
 public:
  /**
   * ThreadPool Constructor
   *
   * Will spawn number of workers based on given number
   *
   * @param threads number of thread workers
   */
  explicit ThreadPool(std::size_t threads);

  /**
   * Add new task to the pool
   *
   * @param f     task function
   * @param args  argument to pass to task function
   *
   * @return async value that returned from task function
   */
  template <class F, class... Args>
  decltype(auto) enqueue(F&& f, Args&&... args);
  /**
   * ThreadPool Destructor
   *
   * Joins all threads
   */
  virtual ~ThreadPool();

 private:
  /**
   * For the sake of keeping track of threads so we can join them
   */
  std::vector<std::thread> workers_;
  /**
   * Task queue
   */
  std::queue<std::packaged_task<void()>> tasks_;
  /**
   * Mutex for synchronization
   */
  std::mutex queue_mutex_;
  /**
   * Condition variable for synchronization
   */
  std::condition_variable condition_;
  /**
   * ThreadPool stop condition
   */
  bool stop_;
};
}  // namespace algo

NAMESPACE_END

#endif  // LIB_ALGO_THREAD_POOL_HPP_
