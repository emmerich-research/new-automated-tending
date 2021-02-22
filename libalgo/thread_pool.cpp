#include "algo.hpp"

#include "thread_pool.hpp"

NAMESPACE_BEGIN

namespace algo {
ThreadPool::ThreadPool(std::size_t threads) : stop_(false) {
  for (size_t i = 0; i < threads; ++i)
    workers_.emplace_back([this] {
      for (;;) {
        std::packaged_task<void()> task;

        {
          std::unique_lock<std::mutex> lock(queue_mutex_);
          condition_.wait(lock, [this] { return stop_ || !tasks_.empty(); });
          if (stop_ && tasks_.empty())
            return;
          task = std::move(tasks_.front());
          tasks_.pop();
        }

        task();
      }
    });
}

void ThreadPool::join() {
  {
    std::unique_lock<std::mutex> lock(queue_mutex_);
    stop_ = true;
  }
  condition_.notify_all();
  for (std::thread& worker : workers_)
    worker.join();
}

ThreadPool::~ThreadPool() {
  join();
}
}  // namespace algo

NAMESPACE_END
