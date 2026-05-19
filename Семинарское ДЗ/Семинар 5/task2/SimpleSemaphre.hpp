#include <atomic>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

#pragma once

using namespace std;
class SimpleSemaphore {
private:
  mutex mtx;
  condition_variable cv;
  int permits;

public:
  SimpleSemaphore(int count) : permits(count) {}

  bool try_acquire_for(int timeout_ms) {

    unique_lock<mutex> lock(mtx);

    bool success = cv.wait_for(lock, chrono::milliseconds(timeout_ms),
                               [&]() { return permits > 0; });

    if (!success)
      return false;

    permits--;

    return true;
  }

  void release() {

    {
      lock_guard<mutex> lock(mtx);
      permits++;
    }

    cv.notify_one();
  }

  void add_permits(int count) {

    {
      lock_guard<mutex> lock(mtx);
      permits += count;
    }

    cv.notify_all();
  }

  int available() {

    lock_guard<mutex> lock(mtx);

    return permits;
  }
};
