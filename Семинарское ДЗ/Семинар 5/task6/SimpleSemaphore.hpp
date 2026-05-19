#pragma once

#include <condition_variable>
#include <mutex>

class SimpleSemaphore {
private:
  std::mutex mtx;

  std::condition_variable cv;

  int permits;

public:
  explicit SimpleSemaphore(int count) : permits(count) {}

  void acquire(int count = 1) {

    std::unique_lock<std::mutex> lock(mtx);

    cv.wait(lock, [&]() { return permits >= count; });

    permits -= count;
  }

  void release(int count = 1) {

    {
      std::lock_guard<std::mutex> lock(mtx);

      permits += count;
    }

    cv.notify_all();
  }

  int available() {

    std::lock_guard<std::mutex> lock(mtx);

    return permits;
  }
};