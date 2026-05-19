#pragma once

#include <chrono>
#include <condition_variable>
#include <mutex>
class SimpleSemaphore {
private:
  std::mutex mtx;

  std::condition_variable cv;

  int permits;

public:
  explicit SimpleSemaphore(int count) : permits(count) {}

  bool try_acquire_for(int timeout_ms) {

    std::unique_lock<std::mutex> lock(mtx);

    bool success = cv.wait_for(lock, std::chrono::milliseconds(timeout_ms),
                               [&]() { return permits > 0; });

    if (!success)
      return false;

    permits--;

    return true;
  }

  void release() {

    {
      std::lock_guard<std::mutex> lock(mtx);

      permits++;
    }

    cv.notify_one();
  }
};
