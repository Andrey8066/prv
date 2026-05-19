#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>

template <typename T> class ThreadSafeQueue {
public:
  void push(const T &value) {
    {
      std::lock_guard lock(mutex_);
      queue_.push(value);
    }

    condition_.notify_one();
  }

  T pop() {
    std::unique_lock lock(mutex_);

    condition_.wait(lock, [this] { return !queue_.empty(); });

    T value = queue_.front();
    queue_.pop();

    return value;
  }

private:
  std::queue<T> queue_;

  std::mutex mutex_;
  std::condition_variable condition_;
};