#ifndef LIB_ALGO_THREAD_POOL_INLINE_HPP_
#define LIB_ALGO_THREAD_POOL_INLINE_HPP_

#include "thread_pool.hpp"

NAMESPACE_BEGIN

namespace algo {
template <class F, class... Args>
decltype(auto) ThreadPool::enqueue(F&& f, Args&&... args) {
  using return_type = std::invoke_result_t<F, Args...>;

  std::packaged_task<return_type()> task(
      std::bind(std::forward<F>(f), std::forward<Args>(args)...));

  std::future<return_type> res = task.get_future();
  {
    std::unique_lock<std::mutex> lock(queue_mutex_);

    // don't allow enqueueing after stopping the pool
    if (stop_) {
      throw std::runtime_error("enqueue on stopped ThreadPool");
    }

    tasks_.emplace(std::move(task));
  }
  condition_.notify_one();
  return res;
}
}  // namespace algo

NAMESPACE_END

#endif  // LIB_ALGO_THREAD_POOL_INLINE_HPP_
